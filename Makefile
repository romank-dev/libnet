#  Copyright 2025 Roman Kudinov. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

PROJECT = $(shell basename $(CURDIR))
SRC_DIR = src

SRC_LIB = src/UdpSocket.cpp src/TcpSocket.cpp src/Socket.cpp src/TcpServerSocket.cpp src/Detector.cpp
SRC_DETECTOR = src/detector.cpp

OBJ_LIB = $(SRC_LIB:$(SRC_DIR)/%.cpp=$(G_OBJ)/$(PROJECT)/%.o)
OBJ_DETECTOR = $(SRC_DETECTOR:$(SRC_DIR)/%.cpp=$(G_OBJ)/$(PROJECT)/%.o)

DEP_LIB = $(OBJ_LIB:%.o=%.d)
DEP_DETECTOR = $(OBJ_DETECTOR:%.o=%.d)

INCLUDE_DIRS = -Iinclude -I$(G_SRC)/libcommon/include 
LIBRARY_DIRS = -L$(G_BIN)
CXXFLAGS = -MMD -fPIC $(INCLUDE_DIRS)  
LDFLAGS = $(LIBRARY_DIRS) -lcommon 

.PHONY: all 
all: $(G_BIN)/libnet.so $(G_BIN)/$(PROJECT)/detector
	@echo "\033[0;97m [DONE] $(PROJECT)"

$(G_BIN)/libnet.so: $(OBJ_LIB)
	@echo "\033[0;32m [LINK] $@ \033[0;0m"
	g++ -shared -o $@ $^ $(LDFLAGS) 
	
$(G_BIN)/$(PROJECT)/detector: $(G_BIN)/libnet.so $(OBJ_DETECTOR)
	@echo "\033[0;32m [LINK] $@ \033[0;0m"
	g++ -o $@ $^ -lnet $(LDFLAGS) 
	

-include $(DEP_LIB)
-include $(DEP_DETECTOR)

$(G_OBJ)/$(PROJECT)/%.o: $(SRC_DIR)/%.cpp
	@echo "\033[0;93m [CC] $@ \033[0;0m"
	g++ -c $< -o $@ $(CXXFLAGS) $(G_CXXFLAGS)

$(shell mkdir -p $(G_BIN)/$(PROJECT))
$(shell mkdir -p $(G_OBJ)/$(PROJECT))
