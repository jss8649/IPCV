/** Implementation file for computing the DFT of a complex vector
 *
 *  \file ipcv/utils/Dft.cpp
 *  \author Jake Stevens (jss8649@rit.edu)
 *  \date 18 Nov 2019
 */

#include "Dft.h"

using namespace std;

namespace ipcv {

std::vector<std::complex<double>> Dft(
    const std::vector<std::complex<double>>& f, int flag) {
    
    const std::complex<double> i(0, 1);
    const double pi = std::acos(-1);
    int N = f.size();
    std::vector<std::complex<double>> F(N);
    
    for(int k = 0; k < f.size(); k++){
        for(int n = 0; n < f.size(); n++){
            if(flag == ipcv::DFT_INVERSE){
                F[k] += f[n]*std::exp(n*k*2*pi/N*i);
            }
            else{
                F[k] += f[n]*std::exp(n*k*-2*pi/N*i);
            }
        }
        if(flag == ipcv::DFT_SCALE){
            F[k] /= N;
        }
    }
  return F;
}
}
