#pragma once

struct PPoint3D
{
	PPoint3D(double x, double y, double z) { p_[0] = x; p_[1] = y; p_[2] = z; }

	double				operator[](int i) const { return p_[i]; }
	double&				operator[](int i) { return p_[i]; }
	double p_[3];
};

struct PTriangle
{
	PTriangle(int i1, int i2, int i3) { i_[0] = i1; i_[1] = i2; i_[2] = i3; }
	int i_[3];
};

typedef std::vector<double>		PRealVector;
typedef std::vector<PPoint3D>	PPoint3DVector;
typedef std::vector<PTriangle>	PTriangleVector;

struct PCurve
{
	PCurve(int degree, PRealVector& knots, PPoint3DVector& cpts) : degree_(degree), knots_(knots), cpts_(cpts) {}
	int				degree_;
	PRealVector		knots_;
	PPoint3DVector	cpts_;
};

struct PMesh
{
	PMesh() {}
	PMesh(PPoint3DVector& pts, PTriangleVector& triangs) : points_(pts), triangles_(triangs) {}

	void					setPoints(const PPoint3DVector &pts) { points_ = pts; }
	void					setTriangles(const PTriangleVector &triangs) { triangles_ = triangs; }

#if 0
	void					clear();
	void					addPoint(const PPoint3D& pnt);
	void					addTriangle(int ind1, int ind2, int ind3);

	const PPoint3DVector&	points() const;
	const PTriangleVector&	triangles() const;
	PPoint3DVector&			points();
	PTriangleVector&		triangles();

	//PTriangle				closestTriangle(const PPoint3D &p) const;
	//bool					writeOBJ(std::string filename) const;

private:
	//PPoint3D				projectToTriangle(const PPoint3D &p, const PTriangle &tri) const;
#endif
	PPoint3DVector			points_;
	PTriangleVector			triangles_;
};
