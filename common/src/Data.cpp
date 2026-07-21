#include "Data.h"

#include <stdexcept>

namespace {
void seekToken(std::istream& input, std::string& token,
               const std::string& expected,
               const std::string& alternative = "") {
    while (input >> token) {
        if (token == expected || (!alternative.empty() && token == alternative)) {
            return;
        }
    }

    throw std::runtime_error("Secao obrigatoria nao encontrada: " + expected);
}

template <typename T>
void readValue(std::istream& input, T& value, const std::string& description) {
    if (!(input >> value)) {
        throw std::runtime_error("Nao foi possivel ler " + description);
    }
}
}

// Construtor atualizado (os vetores se inicializam sozinhos)
Data::Data( int qtParam, char * instance ) {

    if ( qtParam < 2 )  {
        cout << "Missing parameters\n";
        cout << " ./exeLaRP [Instance]"  << endl;
        exit( 1 );        
    }

    if ( qtParam > 2 )    {
        cout << "Too many parameters\n";
        cout << " ./exeLaRP [Instance]"  << endl;
        exit( 1 );
    }

    instanceName = instance;
    nbOfPar = qtParam;
    dimension = -1;
    explicitCoord = false;
}

void Data::read(){

    ifstream inTSP(instanceName, ios::in);

    if ( !inTSP ){
        throw runtime_error("Arquivo de instancia nao encontrado: " + instanceName);
    }

    string file, typeProblem;

    seekToken(inTSP, file, "DIMENSION:", "DIMENSION");

    if ( file.compare("DIMENSION" ) == 0 )  inTSP >> file;

    readValue(inTSP, dimension, "a dimensao da instancia");
    if (dimension <= 0) {
        throw runtime_error("A dimensao da instancia deve ser positiva");
    }

    seekToken(inTSP, file, "EDGE_WEIGHT_TYPE:", "EDGE_WEIGHT_TYPE");
    if ( file.compare("EDGE_WEIGHT_TYPE" ) == 0 )  inTSP >> file;

    readValue(inTSP, typeProblem, "EDGE_WEIGHT_TYPE");

    // Redimensionamento ultra-rápido dos vetores
    xCoord.resize(dimension);
    yCoord.resize(dimension);
    distMatrix.resize(dimension * dimension); // Matriz contígua em 1D

    if ( typeProblem == "EXPLICIT" ) {

        seekToken(inTSP, file, "EDGE_WEIGHT_FORMAT:", "EDGE_WEIGHT_FORMAT");

        string ewf;

        if ( file.compare("EDGE_WEIGHT_FORMAT" ) == 0 )  inTSP >> file;
        readValue(inTSP, ewf, "EDGE_WEIGHT_FORMAT");

        if ( ewf == "FUNCTION" ) {
            throw runtime_error("EDGE_WEIGHT_FORMAT FUNCTION nao suportado");
        }

        else if ( ewf == "FULL_MATRIX" ) {

            seekToken(inTSP, file, "EDGE_WEIGHT_SECTION");

            // Preencher Matriz Distancia Linearizada
            for ( int i = 0; i < dimension; i++ ) {
                for ( int j = 0; j < dimension; j++ ) {
                    int idx = i * dimension + j;
                    inTSP >> distMatrix[idx];
                    if (i == j){
                        distMatrix[idx] = SELF_DISTANCE;
                    }
                }
            }
        }

        else if ( ewf == "UPPER_ROW" ) {

            seekToken(inTSP, file, "EDGE_WEIGHT_SECTION");

            // Preencher Matriz Distancia
            for ( int i = 0; i < dimension; i++ ) {
                for ( int j = i + 1; j < dimension; j++ ) {
                    double val;
                    inTSP >> val;
                    distMatrix[i * dimension + j] = val;
                    distMatrix[j * dimension + i] = val;
                }
            }

            for ( int i = 0; i < dimension; i++ ) {
                distMatrix[i * dimension + i] = SELF_DISTANCE;
            }
        }

        else if ( ewf == "LOWER_ROW" ) {

            seekToken(inTSP, file, "EDGE_WEIGHT_SECTION");

            // Preencher Matriz Distancia
            for ( int i = 1; i < dimension; i++ ) {
                for ( int j = 0; j < i; j++ ) {
                    double val;
                    inTSP >> val;
                    distMatrix[i * dimension + j] = val;
                    distMatrix[j * dimension + i] = val;
                }
            }

            for ( int i = 0; i < dimension; i++ ) {
                distMatrix[i * dimension + i] = SELF_DISTANCE;
            }
        }

        else if ( ewf == "UPPER_DIAG_ROW" ) {

            seekToken(inTSP, file, "EDGE_WEIGHT_SECTION");

            for ( int i = 0; i < dimension; i++ ) {
                for ( int j = i; j < dimension; j++ ) {
                    double val;
                    inTSP >> val;
                    distMatrix[i * dimension + j] = val;
                    distMatrix[j * dimension + i] = val;

                    if (i == j){
                        distMatrix[i * dimension + j] = SELF_DISTANCE;
                    }
                }
            }
        }

        else if ( ewf == "LOWER_DIAG_ROW" ) {

            seekToken(inTSP, file, "EDGE_WEIGHT_SECTION");

            for ( int i = 0; i < dimension; i++ ) {
                for ( int j = 0; j <= i; j++ ) {
                    double val;
                    inTSP >> val;
                    distMatrix[i * dimension + j] = val;
                    distMatrix[j * dimension + i] = val;

                    if (i == j){
                        distMatrix[i * dimension + j] = SELF_DISTANCE;
                    }
                }
            }
        }

        else if ( ewf == "UPPER_COL" ) {

            seekToken(inTSP, file, "EDGE_WEIGHT_SECTION");

            for ( int j = 1; j < dimension; j++ ) {
                for ( int i = 0; i < j; i++ ) {
                    double val;
                    inTSP >> val;
                    distMatrix[i * dimension + j] = val;
                    distMatrix[j * dimension + i] = val;
                }
            }

            for ( int i = 0; i < dimension; i++ ) {
                distMatrix[i * dimension + i] = SELF_DISTANCE;
            }
        }

        else if ( ewf == "LOWER_COL" ) {

            seekToken(inTSP, file, "EDGE_WEIGHT_SECTION");

            for ( int j = 0; j < dimension; j++ ) {
                for ( int i = j + 1; i < dimension; i++ ) {
                    double val;
                    inTSP >> val;
                    distMatrix[i * dimension + j] = val;
                    distMatrix[j * dimension + i] = val;
                }
            }

            for ( int i = 0; i < dimension; i++ ) {
                distMatrix[i * dimension + i] = SELF_DISTANCE;
            }
        }

        else if ( ewf == "UPPER_DIAG_COL" ) {

            seekToken(inTSP, file, "EDGE_WEIGHT_SECTION");

            for ( int j = 0; j < dimension; j++ ) {
                for ( int i = 0; i <= j; i++ ) {
                    double val;
                    inTSP >> val;
                    distMatrix[i * dimension + j] = val;
                    distMatrix[j * dimension + i] = val;
                    if (i == j){
                        distMatrix[i * dimension + j] = SELF_DISTANCE;
                    }
                }
            }
        }

        else if ( ewf == "LOWER_DIAG_COL" ) {

            seekToken(inTSP, file, "EDGE_WEIGHT_SECTION");

            for ( int j = 0; j < dimension; j++ ) {
                // Correção de loop infinito: i < dimension; i++ (estava j++ incorretamente)
                for ( int i = j; i < dimension; i++ ) { 
                    double val;
                    inTSP >> val;
                    distMatrix[i * dimension + j] = val;
                    distMatrix[j * dimension + i] = val;

                    if (i == j){
                        distMatrix[i * dimension + j] = SELF_DISTANCE;
                    }
                }
            }
        } else {
            throw runtime_error("EDGE_WEIGHT_FORMAT nao suportado: " + ewf);
        }
    }

    else if ( typeProblem == "EUC_2D" ) {

        explicitCoord = true;

        seekToken(inTSP, file, "NODE_COORD_SECTION");
        
        int tempCity;
        for ( int i = 0; i < dimension; i++ ) {
            inTSP >> tempCity >> xCoord[i] >> yCoord[i];
        }

        // Calcular Matriz Distancia (Euclidiana) usando .data() no lugar de .get()
        for ( int i = 0; i < dimension; i++ ) {
            for ( int j = 0; j < dimension; j++ ) {
                int idx = i * dimension + j;
                distMatrix[idx] = floor ( CalcDistEuc ( xCoord.data(), yCoord.data(), i, j ) + 0.5 );

                if (i == j){
                    distMatrix[idx] = SELF_DISTANCE;
                }
            }
        }
    }

    else if ( typeProblem == "EUC_3D" ) {
        throw runtime_error("EUC_3D nao suportado"); }

    else if ( typeProblem == "MAX_2D" ) {
        throw runtime_error("MAX_2D nao suportado"); }

    else if ( typeProblem == "MAX_3D" ) {
        throw runtime_error("MAX_3D nao suportado"); }

    else if ( typeProblem == "MAN_2D" ) {
        throw runtime_error("MAN_2D nao suportado"); }

    else if ( typeProblem == "MAN_3D" ) {
        throw runtime_error("MAN_3D nao suportado"); }

    else if ( typeProblem == "CEIL_2D" ) {

        explicitCoord = true;
        seekToken(inTSP, file, "NODE_COORD_SECTION");
        
        int tempCity;
        for ( int i = 0; i < dimension; i++ ) {
            inTSP >> tempCity >> xCoord[i] >> yCoord[i];
        }

        for ( int i = 0; i < dimension; i++ ) {
            for ( int j = 0; j < dimension; j++ ) {
                int idx = i * dimension + j;
                distMatrix[idx] = ceil ( CalcDistEuc ( xCoord.data(), yCoord.data(), i, j ) );

                if (i == j){
                    distMatrix[idx] = SELF_DISTANCE;
                }
            }
        }
    }

    else if ( typeProblem == "GEO" ) {

        explicitCoord = true;

        seekToken(inTSP, file, "NODE_COORD_SECTION");
        
        int tempCity;
        for ( int i = 0; i < dimension; i++ ) {
            inTSP >> tempCity >> xCoord[i] >> yCoord[i];
        }

        // Substituição dos arrays puros por std::vector para evitar vazamentos de memória (leak)
        vector<double> latitude(dimension);
        vector<double> longitude(dimension);

        CalcLatLong ( xCoord.data(), yCoord.data(), dimension, latitude.data(), longitude.data() );

        for ( int i = 0; i < dimension; i++ ) {
            for ( int j = 0; j < dimension; j++ ) {
                int idx = i * dimension + j;
                distMatrix[idx] = CalcDistGeo ( latitude.data(), longitude.data(), i, j );

                if (i == j){
                    distMatrix[idx] = SELF_DISTANCE;
                }
            }
        }
    }

    else if ( typeProblem == "ATT" ) {

        explicitCoord = true;

        seekToken(inTSP, file, "NODE_COORD_SECTION");

        int tempCity;
        vector<int> tempX(dimension);
        vector<int> tempY(dimension);

        for ( int i = 0; i < dimension; i++ ) {
            inTSP >> tempCity >> tempX[i] >> tempY[i];
            xCoord[i] = tempX[i];
            yCoord[i] = tempY[i];
        }

        for ( int i = 0; i < dimension; i++ ) {
            for ( int j = 0; j < dimension; j++ ) {
                int idx = i * dimension + j;
                distMatrix[idx] = CalcDistAtt ( xCoord.data(), yCoord.data(), i, j );

                if (i == j){
                    distMatrix[idx] = SELF_DISTANCE;
                }
            }
        }
    }

    else if ( typeProblem == "XRAY1" ) {
        throw runtime_error("XRAY1 nao suportado"); }

    else if ( typeProblem == "XRAY2" ) {
        throw runtime_error("XRAY2 nao suportado"); }

    else if ( typeProblem == "SPECIAL" ) {
        throw runtime_error("SPECIAL nao suportado"); }
    else {
        throw runtime_error("EDGE_WEIGHT_TYPE nao suportado: " + typeProblem);
    }
}

double Data::CalcDistEuc ( double *X, double *Y, int I, int J ){
    return sqrt ( pow ( X[I] - X[J], 2 ) + pow ( Y[I] - Y[J], 2 ) );
}

double Data::CalcDistAtt ( double *X, double *Y, int I, int J )
{
    double rij, tij, dij;

    rij = sqrt ( ( pow ( X[I] - X[J], 2 ) + pow ( Y[I] - Y[J], 2 ) ) / 10.0 );
    tij = floor ( rij + 0.5 );

    if ( tij < rij )
        dij = tij + 1;
    else
        dij = tij;

    return dij;
}

void Data::CalcLatLong ( double *X, double *Y, int n, double *latit, double* longit )
{
    double PI = 3.141592, min;
    int deg;

    for ( int i = 0; i < n; i++ ) {
        deg = (int) X[i];
        min = X[i] - deg;
        latit[i] = PI * (deg + 5.0 * min / 3.0 ) / 180.0;
    }

    for ( int i = 0; i < n; i++ ) {
        deg = (int) Y[i];
        min = Y[i] - deg;
        longit[i] = PI * (deg + 5.0 * min / 3.0 ) / 180.0;
    }
}

double Data::CalcDistGeo ( double *latit, double *longit, int I, int J )
{
    double q1, q2, q3, RRR = 6378.388;

    q1 = cos( longit[I] - longit[J] );
    q2 = cos( latit[I] - latit[J] );
    q3 = cos( latit[I] + latit[J] );

    return (int) ( RRR * acos( 0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0);
}

string Data::getInstanceName() const
{ 
    string::size_type loc = instanceName.find_last_of(".", instanceName.size() );
    string::size_type loc2 = instanceName.find_last_of("/", instanceName.size() );

    string instance;

    if (loc != string::npos) {
        instance.append(instanceName, loc2+1, loc-loc2-1 );
    } else {
        instance.append(instanceName, loc2+1, instanceName.size() );
    }

    return instance;
}

void Data::printMatrixDist(){
    for (int i  = 1; i <= getDimension(); i++){
        for (int j = 1; j <= getDimension(); j++){
            cout << getDistance(i,j) << " ";
        }
        cout << endl;
    }
}
