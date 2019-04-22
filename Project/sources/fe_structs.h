#pragma once
#include <Eigen/Core>
#include <vector>


struct AssemblyStage {
	bool APPLIED_FORCES, APPLIED_CONSTRAINTS, CONSTRUCTED_MASTER_STIFFNESS;
};

struct AssemblyInfo {


};

struct Ele1D {
	unsigned int n0, n1;
	Ele1D(const unsigned int m0, const unsigned int m1) : n0(m0), n1(m1){}
	Eigen::Matrix4f ElementStiffness;
};

struct FEObject {
	std::vector<Eigen::Vector2f> fNodes;
	std::vector<Ele1D*> fElements;
	Eigen::MatrixXf* MasterStiffness;

	void construct_uniform_element(std::vector<Eigen::Vector2f>& global_element_ind, float A, float E);
	void construct_master_stiffness();
	Eigen::Vector4i get_eft(Ele1D* e);
	FEObject() {};

	~FEObject() {
		for(auto& e : fElements){
			delete e;
		}
		delete MasterStiffness;
	};
};

struct Bar : Ele1D {
	float E, A, L;
	Bar(unsigned int m0, unsigned int m1, float eE, float eA) : Ele1D(m0, m1), E(eE), A(eA) {
		//Element
	}
	void _construct_element_stiffness_matrix(std::vector<Eigen::Vector2f>* nodes);
};

struct EBeam : Ele1D {
	float E, A;
};

struct TBeam : Ele1D {
	float E, A;
};

