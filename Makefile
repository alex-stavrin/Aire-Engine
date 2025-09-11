EXE_FILE=app
SOURCE_DIRECTORY=src
INCLUDE_DIRECTORY=include
OBJDIR=obj
LIBS= -lglfw -lGL -ldl

C_COMPILER=gcc
CPP_COMPILER=g++

HIDE_ERRORS_ON_FIND=2>/dev/null

INCLUDE_FLAGS = $(addprefix -I,${INCLUDE_DIRECTORY} $(shell find ${INCLUDE_DIRECTORY} -type d ${HIDE_ERRORS_ON_FIND}))
C_SOURCE= $(shell find ${SOURCE_DIRECTORY} -type f -name '*.c' ${HIDE_ERRORS_ON_FIND})
CPP_SOURCE= $(shell find ${SOURCE_DIRECTORY} -type f -name '*.cpp' ${HIDE_ERRORS_ON_FIND})
C_OBJS= $(patsubst ${SOURCE_DIRECTORY}/%.c, $(OBJDIR)/%.o, ${C_SOURCE})
CPP_OBJS= $(patsubst $(SOURCE_DIRECTORY)/%.cpp, $(OBJDIR)/%.o, ${CPP_SOURCE})
OBJS = ${C_OBJS} ${CPP_OBJS}
DEPS = $(OBJS:.o=.d)

${EXE_FILE}: $(OBJS)
	${CPP_COMPILER} ${OBJS} -o ${EXE_FILE} ${LIBS}

${OBJDIR}/%.o: ${SOURCE_DIRECTORY}/%.c
	mkdir -p $(@D)
	${C_COMPILER} ${INCLUDE_FLAGS} -c $< -o $@

${OBJDIR}/%.o: ${SOURCE_DIRECTORY}/%.cpp
	mkdir -p $(@D)
	${CPP_COMPILER} ${INCLUDE_FLAGS} -c $< -o $@

-include ${DEPS}

run: ${EXE_FILE}
	./${EXE_FILE}