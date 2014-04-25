#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW_TDM-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1786399364/GestureVariationFollower.o \
	${OBJECTDIR}/_ext/1786399364/init.o \
	${OBJECTDIR}/_ext/1786399364/test_gesture_set.o \
	${OBJECTDIR}/_ext/1786399364/test_mp.o \
	${OBJECTDIR}/_ext/1360910959/YIN.o \
	${OBJECTDIR}/_ext/2102424670/AlphaFilter.o \
	${OBJECTDIR}/_ext/2102424670/utils.o \
	${OBJECTDIR}/DataChecker.o \
	${OBJECTDIR}/GestureSetEvaluation.o \
	${OBJECTDIR}/SyncDataEvaluation.o \
	${OBJECTDIR}/features.o \
	${OBJECTDIR}/newmain.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../../gvf-master/src -L../../../gvf-master/Eigen -L../YIN -L../utils/dist/Debug/MinGW-Windows -lutils

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/supersense.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/supersense.exe: ../utils/dist/Debug/MinGW-Windows/libutils.dll

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/supersense.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/supersense ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/1786399364/GestureVariationFollower.o: nbproject/Makefile-${CND_CONF}.mk ../../../gvf-master/src/GestureVariationFollower.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1786399364
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../gvf-master/Eigen -I../../../gvf-master/src -I../utils -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1786399364/GestureVariationFollower.o ../../../gvf-master/src/GestureVariationFollower.cpp

${OBJECTDIR}/_ext/1786399364/init.o: nbproject/Makefile-${CND_CONF}.mk ../../../gvf-master/src/init.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1786399364
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../gvf-master/Eigen -I../../../gvf-master/src -I../utils -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1786399364/init.o ../../../gvf-master/src/init.cpp

${OBJECTDIR}/_ext/1786399364/test_gesture_set.o: nbproject/Makefile-${CND_CONF}.mk ../../../gvf-master/src/test_gesture_set.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1786399364
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../gvf-master/Eigen -I../../../gvf-master/src -I../utils -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1786399364/test_gesture_set.o ../../../gvf-master/src/test_gesture_set.cpp

${OBJECTDIR}/_ext/1786399364/test_mp.o: nbproject/Makefile-${CND_CONF}.mk ../../../gvf-master/src/test_mp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1786399364
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../gvf-master/Eigen -I../../../gvf-master/src -I../utils -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1786399364/test_mp.o ../../../gvf-master/src/test_mp.cpp

${OBJECTDIR}/_ext/1360910959/YIN.o: nbproject/Makefile-${CND_CONF}.mk ../YIN/YIN.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1360910959
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../gvf-master/Eigen -I../../../gvf-master/src -I../utils -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1360910959/YIN.o ../YIN/YIN.cpp

${OBJECTDIR}/_ext/2102424670/AlphaFilter.o: nbproject/Makefile-${CND_CONF}.mk ../utils/AlphaFilter.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2102424670
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../gvf-master/Eigen -I../../../gvf-master/src -I../utils -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2102424670/AlphaFilter.o ../utils/AlphaFilter.cpp

${OBJECTDIR}/_ext/2102424670/utils.o: nbproject/Makefile-${CND_CONF}.mk ../utils/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2102424670
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../gvf-master/Eigen -I../../../gvf-master/src -I../utils -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2102424670/utils.o ../utils/utils.cpp

${OBJECTDIR}/DataChecker.o: nbproject/Makefile-${CND_CONF}.mk DataChecker.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../gvf-master/Eigen -I../../../gvf-master/src -I../utils -MMD -MP -MF $@.d -o ${OBJECTDIR}/DataChecker.o DataChecker.cpp

${OBJECTDIR}/GestureSetEvaluation.o: nbproject/Makefile-${CND_CONF}.mk GestureSetEvaluation.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../gvf-master/Eigen -I../../../gvf-master/src -I../utils -MMD -MP -MF $@.d -o ${OBJECTDIR}/GestureSetEvaluation.o GestureSetEvaluation.cpp

${OBJECTDIR}/SyncDataEvaluation.o: nbproject/Makefile-${CND_CONF}.mk SyncDataEvaluation.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../gvf-master/Eigen -I../../../gvf-master/src -I../utils -MMD -MP -MF $@.d -o ${OBJECTDIR}/SyncDataEvaluation.o SyncDataEvaluation.cpp

${OBJECTDIR}/features.o: nbproject/Makefile-${CND_CONF}.mk features.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../gvf-master/Eigen -I../../../gvf-master/src -I../utils -MMD -MP -MF $@.d -o ${OBJECTDIR}/features.o features.cpp

${OBJECTDIR}/newmain.o: nbproject/Makefile-${CND_CONF}.mk newmain.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../gvf-master/Eigen -I../../../gvf-master/src -I../utils -MMD -MP -MF $@.d -o ${OBJECTDIR}/newmain.o newmain.cpp

# Subprojects
.build-subprojects:
	cd ../utils && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/supersense.exe

# Subprojects
.clean-subprojects:
	cd ../utils && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
