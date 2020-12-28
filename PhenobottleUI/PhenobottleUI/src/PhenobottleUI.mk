##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=PhenobottleUI
ConfigurationName      :=Debug
WorkspacePath          :=/home/pi/Documents/Phenobottle/PhenobottleUI
ProjectPath            :=/home/pi/Documents/Phenobottle/PhenobottleUI/PhenobottleUI/src
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=
Date                   :=12/23/20
CodeLitePath           :=/home/pi/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="PhenobottleUI.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  $(shell wx-config   --libs --unicode=yes)
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)../include/wxChartDirinclude 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)chartdir 
ArLibs                 :=  "libchartdir" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)../../lib/wxChartDirlib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(shell wx-config --cxxflags --unicode=yes  ) $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(shell wx-config --cxxflags --unicode=yes  ) $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_up_up_up_up_up_usr_local_include_wx_wxchartviewer.cpp$(ObjectSuffix) $(IntermediateDirectory)/SerialComs.cpp$(ObjectSuffix) $(IntermediateDirectory)/mainframe.cpp$(ObjectSuffix) $(IntermediateDirectory)/growlights.cpp$(ObjectSuffix) $(IntermediateDirectory)/csvreader.cpp$(ObjectSuffix) $(IntermediateDirectory)/phframe.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/pi/Documents/Phenobottle/PhenobottleUI/PhenobottleUI/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/up_up_up_up_up_up_up_usr_local_include_wx_wxchartviewer.cpp$(ObjectSuffix): ../../../../../../../usr/local/include/wx/wxchartviewer.cpp $(IntermediateDirectory)/up_up_up_up_up_up_up_usr_local_include_wx_wxchartviewer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/usr/local/include/wx/wxchartviewer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_up_up_up_up_up_usr_local_include_wx_wxchartviewer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_up_up_up_up_up_usr_local_include_wx_wxchartviewer.cpp$(DependSuffix): ../../../../../../../usr/local/include/wx/wxchartviewer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_up_up_up_up_up_usr_local_include_wx_wxchartviewer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_up_up_up_up_up_usr_local_include_wx_wxchartviewer.cpp$(DependSuffix) -MM ../../../../../../../usr/local/include/wx/wxchartviewer.cpp

$(IntermediateDirectory)/up_up_up_up_up_up_up_usr_local_include_wx_wxchartviewer.cpp$(PreprocessSuffix): ../../../../../../../usr/local/include/wx/wxchartviewer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_up_up_up_up_up_usr_local_include_wx_wxchartviewer.cpp$(PreprocessSuffix) ../../../../../../../usr/local/include/wx/wxchartviewer.cpp

$(IntermediateDirectory)/SerialComs.cpp$(ObjectSuffix): SerialComs.cpp $(IntermediateDirectory)/SerialComs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/pi/Documents/Phenobottle/PhenobottleUI/PhenobottleUI/src/SerialComs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SerialComs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SerialComs.cpp$(DependSuffix): SerialComs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SerialComs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SerialComs.cpp$(DependSuffix) -MM SerialComs.cpp

$(IntermediateDirectory)/SerialComs.cpp$(PreprocessSuffix): SerialComs.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SerialComs.cpp$(PreprocessSuffix) SerialComs.cpp

$(IntermediateDirectory)/mainframe.cpp$(ObjectSuffix): mainframe.cpp $(IntermediateDirectory)/mainframe.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/pi/Documents/Phenobottle/PhenobottleUI/PhenobottleUI/src/mainframe.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/mainframe.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/mainframe.cpp$(DependSuffix): mainframe.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/mainframe.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/mainframe.cpp$(DependSuffix) -MM mainframe.cpp

$(IntermediateDirectory)/mainframe.cpp$(PreprocessSuffix): mainframe.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/mainframe.cpp$(PreprocessSuffix) mainframe.cpp

$(IntermediateDirectory)/growlights.cpp$(ObjectSuffix): growlights.cpp $(IntermediateDirectory)/growlights.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/pi/Documents/Phenobottle/PhenobottleUI/PhenobottleUI/src/growlights.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/growlights.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/growlights.cpp$(DependSuffix): growlights.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/growlights.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/growlights.cpp$(DependSuffix) -MM growlights.cpp

$(IntermediateDirectory)/growlights.cpp$(PreprocessSuffix): growlights.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/growlights.cpp$(PreprocessSuffix) growlights.cpp

$(IntermediateDirectory)/csvreader.cpp$(ObjectSuffix): csvreader.cpp $(IntermediateDirectory)/csvreader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/pi/Documents/Phenobottle/PhenobottleUI/PhenobottleUI/src/csvreader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/csvreader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/csvreader.cpp$(DependSuffix): csvreader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/csvreader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/csvreader.cpp$(DependSuffix) -MM csvreader.cpp

$(IntermediateDirectory)/csvreader.cpp$(PreprocessSuffix): csvreader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/csvreader.cpp$(PreprocessSuffix) csvreader.cpp

$(IntermediateDirectory)/phframe.cpp$(ObjectSuffix): phframe.cpp $(IntermediateDirectory)/phframe.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/pi/Documents/Phenobottle/PhenobottleUI/PhenobottleUI/src/phframe.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/phframe.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/phframe.cpp$(DependSuffix): phframe.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/phframe.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/phframe.cpp$(DependSuffix) -MM phframe.cpp

$(IntermediateDirectory)/phframe.cpp$(PreprocessSuffix): phframe.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/phframe.cpp$(PreprocessSuffix) phframe.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


