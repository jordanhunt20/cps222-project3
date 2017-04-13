# Makefile for CPS222 Project 3
# There are three targets that can be made
# * project3 (the default) compiles the student version of the program
# * instructor compiles the instructor version of the program into directory
#   ../instructor, naming the executable instructor_project3
# * distribution creates the distribution to be given to the students in
#   directory project3


# Makefiles define rules for making files from other files.
# Rules have the a common form.
#    target:   dependency list
#        command to make target
# Note that the command is only run if the target is older than
# its dependencies.  For example, if a binary is older than the sources
# it is made from, it needs to be recompiled.

# Tabs are important in makefiles.
# There must be a tab after the ":".
# There must be a tab before each command.

# This rule makes the binary program from .o files.
# (.o files are made from .cc files using a rule further down.)
# The command uses some special variables:
#   $@  -- the name of the target
#   $^  -- the names of all dependencies (the .o files listed after ":")
project3: project3.o project3tester.o
	g++ -o $@ $^

project3.cc:	project3.h

# This is a pattern matching rule.  Any file ending in .o can be made from
# a matching .cc file (note the "%"s).  The command uses a special variable:
#   $<  -- the first dependency (that is, the .cc file)
%.o:		%.cc
	g++ -c $<

instructor: instructor_project3.cc project3tester.o
	rm -rf ../instructor
	mkdir ../instructor
	g++ -o ../instructor/instructor_project3 instructor_project3.cc project3tester.o
	cp testdemo.in ../instructor

distribution: project3.cc project3.h project3tester.cc
	echo 'Be sure this is being done on the architecture students will use in the lab'
	rm -rf ../project3
	mkdir ../project3
	g++ -c project3tester.cc
	cp project3.cc ../project3
	cp project3.h ../project3
	mv project3tester.o ../project3
	cp testdemo.in ../project3
	cp makefile ../project3
	chmod -R a+rX ../project3
