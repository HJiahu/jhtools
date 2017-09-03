macro = -DUSE_OPENCV  -fpermissive 
lib_path = /usr/lib/
caffe_include = /opt/yuwei/caffe-master/include
caffe_lib_path = /opt/yuwei/caffe-master/build/lib
jhtools_path = /home/t704/Desktop/gedc14/jhtools
cuda_include = /usr/local/cuda/include
opencv_lib_linker = -lopencv_core -lopencv_highgui -lopencv_imgproc 
other_lib_linker =-lcaffe     -lgflags -lglog -lprotobuf  -lpthread -lboost_system 

classifier:*.cpp
	g++  $(macro) -oclassifier.out -std=c++11 -I$(cuda_include) -I$(caffe_include)  -I$(jhtools_path) *.cpp $(jhtools_path)/jhtools.cpp -L$(caffe_lib_path)  $(opencv_lib_linker) $(other_lib_linker) 
