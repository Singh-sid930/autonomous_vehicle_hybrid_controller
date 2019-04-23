////at the end create a function out of this : input is max_curvature, way_pts : output is way_traj 

#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <Eigen/Dense>

#define PI 3.14159265

#define max_curvature 2
#define center_line_dt 0.01
#define way_pt_no 4


using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace Eigen;
using namespace std;



// double way_x[6]={ 0, 00, 10, 10, 40, 70 };
// double way_y[6]={ 0, 10, 10, 30, 30, 00 };


Vector2d way_pt[way_pt_no]; 

int temp = ((way_pt_no -2)*2/(center_line_dt*2) + (way_pt_no -1)*200);
Vector2d way_traj[(int)((way_pt_no -2)*2/center_line_dt + (way_pt_no -1)*200)];



int main(void)
{	

	cout<<temp<<endl;
	way_pt[0] <<  0, 0;
	way_pt[1] <<  6.5, -2.48;
	way_pt[2] <<  20.75, 10;
    way_pt[3] <<  20, 35.6;
	// way_pt[4] <<  20, 10;
	// way_pt[5] <<  30, 10;

	// Vector2d way_last;
	// way_last << 0.1,0.1;

	clock_t tStart = clock();
	ofstream myfile;
  	myfile.open ("example.txt"); 			

	Vector2d W1, W2, W3, U1, U2, Ud, B0, B1, B2, B3, E0, E1, E2, E3;	

	double c1=7.2364, c2=(sqrt(6)-1)*2/5, c3, kmax=max_curvature;  
	double beta, gammaa, dk, hb, he, gb, ge, kb, ke ;

	int ways=way_pt_no,traj_pt=0;

	c3 = (c2+4)/(c1+6);

	int i=0;
	double temp_slope1,temp_slope2;
	double t=0;

	way_traj[traj_pt] << 0,0;
	Vector2d temp_pos = way_traj[traj_pt], temp_pos1;
	traj_pt++;
		
	

	while(i < (ways-2))
	{	


		// W[0][0]= way_x[i]; W[0][1]= way_y[i]; 
		// W[1][0]= way_x[i+1]; W[1][1]= way_y[i+1];
		// W[2][0]= way_x[i+2]; W[2][1]= way_y[i+2];	
		
		// temp_slope1 = atan((way_pt[i+1](1)-way_pt[i](1))/(way_pt[i+1](0)-way_pt[i](0))); 	
		// temp_slope2 = atan((way_pt[i+2](1)-way_pt[i+1](1))/(way_pt[i+2](0)-way_pt[i+1](0))); 	


		W1 = way_pt[i];
		W2 = way_pt[i+1];
		W3 = way_pt[i+2];

		// W1 = way_pt[1];
		// W2 = way_pt[2];
		// W3 = way_pt[3];
		// //cout<<" W1 " << W1 <<" W2 "<<W2 <<" W3 "<< W3<<endl; 

		///////////////old ////////////////////
		// temp_slope1 = atan((W2(1) - W1(1))/(W2(0) - W1(0)));	
		// temp_slope2 = atan((W3(1) - W2(1))/(W3(0) - W2(0)));
		// //cout << temp_slope1 <<" "<< temp_slope1<<endl;	

		// if (temp_slope1<0)
		// 	temp_slope1=temp_slope1+PI;

		// if (temp_slope2<0)
		// 	temp_slope2=temp_slope2+PI;
		// gammaa = fabs(temp_slope2 - temp_slope1);
		// beta = gammaa/2;
		////////////////////////////////////////////

		Vector3d MW21, MW32, gamma_cross_vector;
		MW21 << (W2(0)-W1(0)), (W2(1)-W1(1)), (0);
		MW32 << (W3(0)-W2(0)), (W3(1)-W2(1)), (0);

		gamma_cross_vector = MW21.cross(MW32);

		gammaa = asin((gamma_cross_vector.norm())/(((W2-W1).norm())*(((W3-W2).norm()))));
		
		beta = (PI-gammaa)/2;
		dk = (pow(c2+4,2)*sin(beta))/(54*c3*kmax*pow(cos(beta),2));
		hb=he=c3*dk; 
		gb=ge=c2*c3*dk;
		kb=ke=(6*c3*cos(beta)*dk)/(c2+4);	

		//U1 = (W[1]-W[2])/(W[1].norm() * W[2].norm());
		//U2 = (W[2]-W[3])/(W[2].norm() * W[3].norm());

		U1 = (W1-W2)/((W1-W2).norm());
		U2 = (W3-W2)/((W3-W2).norm());
		
		B0 = W2 + dk*U1;
		B1 = B0 - gb*U1;
		B2 = B1 - hb*U1;
	
		E0 = W2 + dk*U2;
		E1 = E0 - ge*U2;
		E2 = E1 - he*U2;

		Ud = (E2-B2)/((E2-B2).norm());

		B3 = B2 + kb*Ud;
		E3 = E2 - ke*Ud;


		// cout << "Here is the vector B0:\n" << B[0] << endl;	
		// cout << "Here is the vector B1:\n" << B[1] << endl;	
		// cout << "Here is the vector B2:\n" << B[2] << endl;



		////interpolation code 

		temp_pos1 = (pow(1-t,3)*B0 + 3*pow(1-t,2)*t*B1 + 3*(1-t)*pow(t,2)*B2 + pow(t,3)*B3);
		//cout<<"temp_pos1 "<<temp_pos1<<endl;

		//if(traj_pt==1)
		{	
		
			if(( temp_pos1 - temp_pos ).norm() > 0.2)
			{
				Vector2d temp_step = (temp_pos1 - temp_pos)/200;
				for(int x=0; x<200; x++)
				{
					way_traj[traj_pt] = way_traj[traj_pt-1] + temp_step;
					//cout <<way_traj[traj_pt](0)<<" "<<way_traj[traj_pt](1)<< endl;
					myfile <<way_traj[traj_pt](0)<<" "<<way_traj[traj_pt](1)<< endl; 
					traj_pt++;	
				}

			}
		}
		////////////////////
			
		while(t<=1)
		{	
			way_traj[traj_pt] = (pow(1-t,3)*B0 + 3*pow(1-t,2)*t*B1 + 3*(1-t)*pow(t,2)*B2 + pow(t,3)*B3); 	
			//cout <<way_traj[traj_pt](0)<<" "<<way_traj[traj_pt](1)<< endl;
			myfile <<way_traj[traj_pt](0)<<" "<<way_traj[traj_pt](1)<< endl;			
			traj_pt++;
			t += center_line_dt*2;
		}	
		
		while(t>=0)
		{	
			way_traj[traj_pt] = (pow(1-t,3)*E0 + 3*pow(1-t,2)*t*E1 + 3*(1-t)*pow(t,2)*E2 + pow(t,3)*E3); 	
			//cout <<way_traj[traj_pt](0)<<" "<<way_traj[traj_pt](1)<< endl;
			myfile <<way_traj[traj_pt](0)<<" "<<way_traj[traj_pt](1)<< endl;
			traj_pt++;
			t -= center_line_dt*2;
		}


		temp_pos =  way_traj[traj_pt-1];
		//cout << "temp_pos " <<temp_pos<<endl;
		i++;
	}


		////interpolation code 

		temp_pos1 = way_pt[way_pt_no-1];
		//cout<<"temp_pos1 "<<temp_pos1<<endl;

		//if(traj_pt==1)
		{	
		
			if(( temp_pos1 - temp_pos ).norm() > 0.2)
			{
				Vector2d temp_step = (temp_pos1 - temp_pos)/200;
				for(int x=0; x<200; x++)
				{
					way_traj[traj_pt] = way_traj[traj_pt-1] + temp_step;
					//cout <<way_traj[traj_pt](0)<<" "<<way_traj[traj_pt](1)<< endl;
					myfile <<way_traj[traj_pt](0)<<" "<<way_traj[traj_pt](1)<< endl; 
					traj_pt++;	
				}

			}
		}
		////////////////////

	myfile.close();
	printf("Exec Time: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	return 0;
}