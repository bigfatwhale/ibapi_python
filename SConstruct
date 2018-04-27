import os
import os.path

boost_loc = os.environ['BOOST_ROOT']
anaconda_loc = os.environ['ANACONDA_ROOT']

def buildSymlink( target, source, env ):
    if not os.path.exists('ibapi.so'):    
        os.system('ln -s libibapi.dylib ibapi.so')

env = Environment( CCFLAGS = '-std=c++11', 
                   CPPPATH = ['.'], 
                   LIBPATH = [ boost_loc + '/stage/lib', 
                               anaconda_loc + '/lib'] )

buildLib = env.SharedLibrary('ibapi', Glob('*.cpp'), LIBS=['boost_python', 'python2.7'])
#env.AddPostAction( buildLib, 'ln -s libibapi.dylib ibapi.so' )
env.AddPostAction( buildLib, buildSymlink )  

Repository(boost_loc)
Repository( anaconda_loc + '/include/python2.7')
