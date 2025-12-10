#!/usr/bin/env python

"""
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SConstruct                                                            
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                         This file is part of:                          
                             GDSynthesizer                              
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Copyright (c) 2023-2024 Soyo Kuyo.                                     
                                                                        
 Permission is hereby granted, free of charge, to any person obtaining  
 a copy of this software and associated documentation files (the        
 "Software"), to deal in the Software without restriction, including    
 without limitation the rights to use, copy, modify, merge, publish,    
 distribute, sublicense, and/or sell copies of the Software, and to     
 permit persons to whom the Software is furnished to do so, subject to  
 the following conditions:                                              
                                                                        
 The above copyright notice and this permission notice shall be         
 included in all copies or substantial portions of the Software.        
                                                                        
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
"""

import os
import sys
 
project_name = 'libgdsynthesizer'
env = SConscript('./godot-cpp-4.2.1-stable/SConstruct')
 
print('build target:', type(env['target']), env['target'])
print("env['CCFLAGS']:", type(env['CCFLAGS']), env['CCFLAGS'])
print("env['CFLAGS']:", type(env['CFLAGS']), env['CFLAGS'])
print("env['CXXFLAGS']:", type(env['CXXFLAGS']), env['CXXFLAGS'])
print("env['CPPDEFINES']:", type(env['CPPDEFINES']), env['CPPDEFINES'])
print("env['LINKFLAGS']:", type(env['LINKFLAGS']), env['LINKFLAGS'])
print("env['CPPPATH']:")
for i in env['CPPPATH']:
    print("  ",type(i), i)
print("env['LIBS']:")
for i in env['LIBS']:
    print("  ",type(i), i)
env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")
 
if env['platform'] == "windows" and env['arch'] == "x86_64" and env.get('use_mingw', False):
    env.Append(LINKFLAGS = ['-static-libgcc', '-static-libstdc++','-static','-pthread'])
elif env['platform'] == "web" and env['arch'] == "wasm32":
    # strip -fno-exceptions from $CXXFLAGS.
#    env['CXXFLAGS'] = SCons.Util.CLVar(str(env['CXXFLAGS']).replace("-fno-exceptions", ""))
    print("env['CXXFLAGS']", env['CXXFLAGS'])
else:
    print("not suppoted conbination!")
    print("platform",env["platform"])
    print("use_mingw",env["use_mingw"])
    print("use_clang_cl",env["use_clang_cl"])
    print("use_static_cpp",env["use_static_cpp"])
    print("android_api_level",env["android_api_level"])
    print("ios_simulator",env["ios_simulator"])
    print("arch",env["arch"])
    print("expected: windows + use_mingw=yes  OR  web + wasm32")
    sys.exit(1)
 
print("CPPDEFINES",env['CPPDEFINES'])
library = env.SharedLibrary(
    "project/bin/{}{}{}".format(project_name,env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)
 
Default(library)
