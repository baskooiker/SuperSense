/* 
 * File:   DataChecker.h
 * Author: Bas
 *
 * Created on 1 april 2014, 19:07
 */

#ifndef DATACHECKER_H
#define	DATACHECKER_H

#include "GestureSetEvaluation.h"

#include <vector>
#include <string>

class DataChecker : private GestureSetEvaluation{
public:
    DataChecker();
    DataChecker(const DataChecker& orig);
    virtual ~DataChecker();
    
    bool checkCorrectness(std::vector<string> filenames);
private:

};

#endif	/* DATACHECKER_H */

