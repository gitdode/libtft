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
CC=avr-gcc
CCC=avr-g++
CXX=avr-g++
FC=gfortran
AS=avr-as

# Macros
CND_PLATFORM=AVR-Linux
CND_DLIB_EXT=so
CND_CONF=Custom
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include /home/dode/dev/libtft/Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/5ac08bf9/font.o \
	${OBJECTDIR}/_ext/5ac08bf9/hack.o \
	${OBJECTDIR}/_ext/5ac08bf9/libtft.o \
	${OBJECTDIR}/_ext/5ac08bf9/unifont.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtft

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtft: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtft ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/5ac08bf9/font.o: /home/dode/dev/libtft/font.c
	${MKDIR} -p ${OBJECTDIR}/_ext/5ac08bf9
	$(COMPILE.c) -g -D__flash=volatile -o ${OBJECTDIR}/_ext/5ac08bf9/font.o /home/dode/dev/libtft/font.c

${OBJECTDIR}/_ext/5ac08bf9/hack.o: /home/dode/dev/libtft/hack.c
	${MKDIR} -p ${OBJECTDIR}/_ext/5ac08bf9
	$(COMPILE.c) -g -D__flash=volatile -o ${OBJECTDIR}/_ext/5ac08bf9/hack.o /home/dode/dev/libtft/hack.c

${OBJECTDIR}/_ext/5ac08bf9/libtft.o: /home/dode/dev/libtft/libtft.c
	${MKDIR} -p ${OBJECTDIR}/_ext/5ac08bf9
	$(COMPILE.c) -g -D__flash=volatile -o ${OBJECTDIR}/_ext/5ac08bf9/libtft.o /home/dode/dev/libtft/libtft.c

${OBJECTDIR}/_ext/5ac08bf9/unifont.o: /home/dode/dev/libtft/unifont.c
	${MKDIR} -p ${OBJECTDIR}/_ext/5ac08bf9
	$(COMPILE.c) -g -D__flash=volatile -o ${OBJECTDIR}/_ext/5ac08bf9/unifont.o /home/dode/dev/libtft/unifont.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
