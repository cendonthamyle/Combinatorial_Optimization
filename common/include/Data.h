#ifndef DATA_H
#define DATA_H

constexpr double SELF_DISTANCE = 0.0;

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

class Data {
public:
    Data( int, char * );

    void read();
    void printMatrixDist();
    
    inline int getDimension() const { return dimension; }
    inline double getDistance(int i, int j) const {
        return distMatrix[(i - 1) * dimension + (j - 1)]; 
    }
    inline const double* getMatrixCost() const { return distMatrix.data(); }
    inline double getXCoord(int i) const { return xCoord[i - 1]; }
    inline double getYCoord(int i) const { return yCoord[i - 1]; }
    inline bool getExplicitCoord() const { return explicitCoord; }

    string getInstanceName() const;

    private:
        string instanceName;
        int nbOfPar;
        int dimension;

        vector<double> distMatrix; 
        vector<double> xCoord;
        vector<double> yCoord;

        // Computing Distances
        static double CalcDistEuc ( double *, double *, int , int );
        static double CalcDistAtt ( double *, double *, int , int );
        static double CalcDistGeo ( double *, double *, int , int );
        static void CalcLatLong ( double *, double *, int , double *, double* );

        bool explicitCoord;
    };

#endif
