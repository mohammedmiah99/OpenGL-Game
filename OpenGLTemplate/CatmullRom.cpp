#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>



CCatmullRom::CCatmullRom()
{

}

CCatmullRom::~CCatmullRom()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f*p1 - 3.0f*p2 + p3);

	return a + b*t + c*t2 + d*t3;

}


void CCatmullRom::SetControlPoints()
{
	// Set control points (m_controlPoints) here, or load from disk
	m_controlPoints.push_back(glm::vec3(350, 40, -125));
	m_controlPoints.push_back(glm::vec3(300, 40, 30));
	m_controlPoints.push_back(glm::vec3(300, 50, 100));
	m_controlPoints.push_back(glm::vec3(250, 50, 100));
	m_controlPoints.push_back(glm::vec3(200, 40, 80));
	m_controlPoints.push_back(glm::vec3(70, 20, 71));
	m_controlPoints.push_back(glm::vec3(0, 20, 100));
	m_controlPoints.push_back(glm::vec3(-101, 5, 150));
	m_controlPoints.push_back(glm::vec3(-150, 5, 0));
	m_controlPoints.push_back(glm::vec3(-71, 5, -71));
	m_controlPoints.push_back(glm::vec3(0, 5, -100));
	m_controlPoints.push_back(glm::vec3(71, 5, -71));
	
	// Optionally, set upvectors (m_controlUpVectors, one for each control point as well)
	//m_controlUpVectors.push_back(glm::vec3(10.0, 10.0, 10.0));
}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int) m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i-1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M-1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3 &p, glm::vec3 &up)
{
	if (d < 0)
		return false;

	int M = (int) m_controlPoints.size();
	if (M == 0)
		return false;


	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int) (d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size(); i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;
	
	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j-1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point (and upvector)
	p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
	if (m_controlUpVectors.size() == m_controlPoints.size())
		up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));


	return true;
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);

	}


	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}


}



void CCatmullRom::CreateCentreline()
{
	// Call Set Control Points
	SetControlPoints();

	// Call UniformlySampleControlPoints with the number of samples required
	UniformlySampleControlPoints(500);

	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
	
	//GLuint m_vaoCentreline;
	glGenVertexArrays(1, &m_vaoCentreline);
	glBindVertexArray(m_vaoCentreline);
	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < 500; i++) {
		float t = (float)i / 1.0f;
		glm::vec3 v = m_centrelinePoints[t];
		vbo.AddData(&v, sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}
	//Upload to GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	
	//vertex pos, texture coordinates, normal vectors
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}


void CCatmullRom::CreateOffsetCurves()
{
	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively

		// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
		// Note it is possible to only use one VAO / VBO with all the points instead.

	glm::vec3 p;
	glm::vec3 pNext;
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 t;

	for (unsigned int i = 0; i < m_centrelinePoints.size(); i++) {
		p = m_centrelinePoints[i];
		if (i == m_centrelinePoints.size() - 1) 
			pNext = m_centrelinePoints[0];
		else 
			pNext = m_centrelinePoints[i + 1];
			t = glm::normalize(pNext - p);
			glm::vec3 n = glm::normalize(glm::cross(t, up));

		//Push back each point for both left and right
		m_leftOffsetPoints.push_back(m_centrelinePoints[i] - ((m_pathWidth / 2) * n));
		m_rightOffsetPoints.push_back(m_centrelinePoints[i] + ((m_pathWidth / 2) * n));
	}

	//VAO for left
	glGenVertexArrays(1, &m_vaoLeftOffsetCurve);
	glBindVertexArray(m_vaoLeftOffsetCurve);

	//VBO for left offset
	CVertexBufferObject vboleft;
	vboleft.Create();
	vboleft.Bind();

	glm::vec2 lefttex(0.0f, 0.0f);
	glm::vec3 leftn(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < m_leftOffsetPoints.size(); i++)
	{
		glm::vec3 v = m_leftOffsetPoints[i];
		vboleft.AddData(&v, sizeof(glm::vec3));
		vboleft.AddData(&lefttex, sizeof(glm::vec2));
		vboleft.AddData(&leftn, sizeof(glm::vec3));
	}

	//Upload to GPU
	vboleft.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei v = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	
	//vertex pos, texture coordinates, normal vectors
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, v, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, v, (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, v, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	// VAO for right
	glGenVertexArrays(1, &m_vaoRightOffsetCurve);
	glBindVertexArray(m_vaoRightOffsetCurve);

	//VBO for right offset
	CVertexBufferObject vboright;
	vboright.Create();
	vboright.Bind();

	glm::vec2 righttex(0.0f, 0.0f);
	glm::vec3 rightn(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < m_rightOffsetPoints.size(); i++)
	{
		glm::vec3 vr = m_rightOffsetPoints[i];
		vboright.AddData(&vr, sizeof(glm::vec3));
		vboright.AddData(&righttex, sizeof(glm::vec2));
		vboright.AddData(&rightn, sizeof(glm::vec3));
	}
	//Upload to GPU
	vboright.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei s = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);

	//vertex pos, texture coordinates, normal vectors
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, s, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, s, (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, s, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}


void CCatmullRom::CreateTrack()
{
	// Load the texture
	m_texture.Load("resources\\textures\\rr.jpg", true);

	//Set Sampler Object Parameters
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);

	// Create a VBO
	CVertexBufferObject vaoTrack;
	vaoTrack.Create();
	vaoTrack.Bind();

	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec2 texCoord1(0.0f, 1);
	glm::vec2 texCoord2(0.1f, 0);
	glm::vec2 texCoord3(0.1f, 1);

	for (int i = 0; i < m_rightOffsetPoints.size() - m_pathVertexOffset; i += m_pathVertexOffset)
	{
		m_pathpp.push_back(m_leftOffsetPoints[i]);
		m_path.push_back(texCoord);
		m_pathpp.push_back(m_rightOffsetPoints[i]);
		m_path.push_back(texCoord1);
		m_pathpp.push_back(m_leftOffsetPoints[i + m_pathVertexOffset]);
		m_path.push_back(texCoord2);
		m_pathpp.push_back(m_rightOffsetPoints[i + m_pathVertexOffset]);
		m_path.push_back(texCoord3);
	}

	glm::vec3 n(0.0f, 1.0f, 0.0f);

	for (unsigned int i = 0; i < m_pathpp.size(); i++)
	{
		vaoTrack.AddData(&m_pathpp[i], sizeof(glm::vec3));
		vaoTrack.AddData(&m_path[i], sizeof(glm::vec2));
		vaoTrack.AddData(&n, sizeof(glm::vec3));
	}

	// Upload to the GPU
	vaoTrack.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei ss = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	
	//vertex pos, texture coordinates, normal vectors
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, ss, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, ss, (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, ss, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	
}


void CCatmullRom::RenderCentreline()
{
	// Bind the VAO m_vaoCentreline and render
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoCentreline);
	glDrawArrays(GL_POINTS,0, m_centrelinePoints.size());
	glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());
}

void CCatmullRom::RenderOffsetCurves()
{
	// Bind the VAO m_vaoLeftOffsetCurve and render
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoLeftOffsetCurve);
	glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());
	glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());
	// Bind the VAO m_vaoRightOffsetCurve and render it
	glLineWidth(5.0f);
	glBindVertexArray(m_vaoRightOffsetCurve);
	glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());
	glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());
}


void CCatmullRom::RenderTrack()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Bind the VAO m_vaoTrack and render
	glBindVertexArray(m_vaoTrack);
	m_texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_pathpp.size());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int CCatmullRom::CurrentLap(float d)
{

	return (int)(d / m_distances.back());

}