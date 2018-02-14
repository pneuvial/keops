/* Based on the work of J. Glaunes */
/* Authors : this file is part of the fshapesTk by B. Charlier, N. Charon, A. Trouve (2012-2014) */

typedef  __TYPE__(*KernelFun)( __TYPE__,  __TYPE__);

///////////////////////
//  Gaussian Kernel  //
///////////////////////

template < typename TYPE >
__device__ TYPE GaussF(TYPE r2, TYPE ooSigma2) {
    // Gaussian radial kernel - takes as input the squared norm r2
    return exp(- r2 * ooSigma2);
}

template < typename TYPE >
__device__ TYPE GaussFp(TYPE r2, TYPE ooSigma2) {
    // Gaussian radial kernel - 1st derivative
    return - ooSigma2 * exp(- r2 * ooSigma2);
}

template < typename TYPE >
__device__ TYPE GaussFpp(TYPE r2, TYPE ooSigma2) {
    // Gaussian radial kernel - 2nd derivative
    return + ooSigma2 * ooSigma2 * exp(- r2 * ooSigma2);
}

///////////////////////
//  Laplace Kernel  //
///////////////////////

template < typename TYPE >
__device__ TYPE LaplaceF(TYPE r2, TYPE ooSigma2) {
    // Laplace radial kernel - takes as input the squared norm r2
    return exp(- sqrt( 1.0f/ooSigma2 + r2));
}

template < typename TYPE >
__device__ TYPE LaplaceFp(TYPE r2, TYPE ooSigma2) {
    // Laplace radial kernel - 1st derivative
    TYPE s = sqrt( 1.0f/ooSigma2 + r2);
    return - exp(- s ) / (2.0f * s );
}

template < typename TYPE >
__device__ TYPE LaplaceFpp(TYPE r2, TYPE ooSigma2) {
    // Laplace radial kernel - 2nd derivative
    TYPE s = sqrt( 1.0f/ooSigma2 + r2);
    return .25f * (1.0f/(s*s*s) + 1.0f/s) * exp(- s);
}


///////////////////////
//   Energy  Kernel  //
///////////////////////

template < typename TYPE >
__device__ TYPE EnergyF(TYPE r2, TYPE ooSigma2) {
    // Energy radial kernel - takes as input the squared norm r2
    return 1.0f / powf( 1.0f/ooSigma2 + r2, .25f);
}

template < typename TYPE >
__device__ TYPE EnergyFp(TYPE r2, TYPE ooSigma2) {
    // Energy radial kernel - 1st derivative
    return -.25f / powf( 1.0f/ooSigma2 + r2, 1.25f);
}

template < typename TYPE >
__device__ TYPE EnergyFpp(TYPE r2, TYPE ooSigma2) {
    // Energy radial kernel - 2nd derivative
    return .3125f / powf( 1.0f/ooSigma2 + r2, 2.25f);
}


///////////////////////
//  Legacy  Kernels  //
///////////////////////

template < typename TYPE >
__device__ TYPE KernelGauss(TYPE *u, TYPE *v, TYPE ooSigma2, int DIM) {

    TYPE r2 = 0.0f;
    TYPE temp;
// norm squared
    for(int k=0; k<DIM; k++) {
        temp =  v[k]-u[k];
        r2 += temp*temp;
    }

// Gaussian radial kernel
    return exp(- r2 * ooSigma2);

}


template < typename TYPE >
__device__ TYPE Kerneld1Gauss(TYPE *u, TYPE *v, TYPE ooSigma2, int l, int DIM) {

    TYPE r2 = 0.0f;
    TYPE temp;
// norm squared
    for(int k=0; k<DIM; k++) {
        temp =  v[k]-u[k];
        r2 += temp*temp;
    }

// Gaussian radial kernel
    return -2 * ooSigma2 * (v[l] - u[l]) * exp(- r2 * ooSigma2);

}



//////////////////////////
//  Kernel on Varifold  //
//////////////////////////

template < typename TYPE >
__device__ TYPE KernelGaussVar(TYPE *u, TYPE *v, TYPE ooSigma2, int DIM) {

    TYPE temp;

// norm(u) squared
    TYPE normu2 = 0.0f;
    for(int k=0; k<DIM; k++) {
        temp = u[k]*u[k];
        normu2 += temp;
    }

// norm(v) squared
    TYPE normv2 = 0.0f;
    for(int k=0; k<DIM; k++) {
        temp =  v[k]*v[k];
        normv2 += temp;
    }

// norm(u) squared
    TYPE prsuv = 0.0f;
    for(int k=0; k<DIM; k++) {
        temp =  v[k]*u[k];
        prsuv += temp;
    }

    temp = (normu2*normv2);

// Gaussian kernel
    return sqrt(temp)*exp(2.0f * (prsuv*prsuv /temp)* ooSigma2);

}