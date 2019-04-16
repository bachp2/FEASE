#pragma once
#include <Eigen/Core>
std::vector<Eigen::Vector2f> FENodes;
struct Ele1D {
	unsigned int n0, n1;
	Ele1D(const unsigned int m0, const unsigned int m1) : n0(m0), n1(m1){}
private:
	Eigen::Matrix4f ElementStiffness;
};

struct Bar : Ele1D {
	float E, A, L;
	Bar(unsigned int m0, unsigned int m1, float eE, float eA) : Ele1D(m0, m1), E(eE), A(eA) {
		//Element

	}
};



struct EBeam : Ele1D {
	float E, A;
};

struct TBeam : Ele1D {
	float E, A;
};

struct FEObject {
	

};