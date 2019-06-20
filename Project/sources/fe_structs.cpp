#include "fe_structs.h"
#include <iostream>
void Bar::_construct_element_stiffness_matrix(std::vector<Eigen::Vector2f>* nodes)
{

	//Eigen::Vector2f diffvec = (*nodes)[n1] - (*nodes)[n0];
	//float L = diffvec.norm();
	//this->L = L;
	//auto c = diffvec[0] / L; // cosine theta
	//auto s = diffvec[1] / L; // sine theta
	//auto cc = c*c;
	//auto cs = c*s;
	//auto ss = s*s;
	//ElementStiffness << cc, cs, -cc, -cs,
	//					cc, cs, -cc, -cs,
	//					-cc, -cs, cc, cs,
	//					-cs, -ss, cs, ss;
	//ElementStiffness *= E*A/L;
	//std::cout << ElementStiffness;
}

void FEObject::construct_uniform_element(std::vector<Eigen::Vector2f>& global_element_ind, float A, float E)
{
	/*for(auto& e : global_element_ind){
		Ele1D* bele = new Bar(e[0], e[1], E, A);
		((Bar*)bele)->_construct_element_stiffness_matrix(&fNodes);
		fElements.push_back(bele);
	}*/
}

void FEObject::construct_master_stiffness()
{
	/*auto n = fNodes.size()*2;
	MasterStiffness = new Eigen::MatrixXf(n,n);

	printf("* Constructing Master Stiffness Matrix...");
	for(auto &e : fElements){
		auto eft = get_eft(e);
		for (int i = 0; i < 4; i++){
			auto ii = eft[i];
			for (int j = i; j < 4; j++){
				auto jj = eft[j];
				(*MasterStiffness)(ii, jj) += ((Bar*)e)->ElementStiffness(i, j);
				(*MasterStiffness)(jj, ii) = (*MasterStiffness)(ii, jj);
			}
		}
	}
	printf("Done\n");
	asm_info.CONSTRUCTED_MASTER_STIFFNESS = true;*/
}

Eigen::Vector4i FEObject::get_eft(Ele1D* e)
{
	Eigen::Vector4i eft;
	eft << 2*e->n0, 2*e->n0+1, 2*e->n1, 2*e->n1+1;
	return eft;
}