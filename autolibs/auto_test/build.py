import os
import sys

def GetAutoLibDirList():
    autoLibRoot = os.path.join( os.getcwd(), '..' )
    result = []
    for dir in os.listdir( autoLibRoot ):
        if not dir.startswith( 'auto_' ):
            continue
        if dir == 'auto_test':
            continue
        if os.path.isdir( os.path.join( autoLibRoot, dir )):
            result.append( dir )
    return result

def GetSourceList():
    autoLibRoot = os.getcwd()
    result = []
    for src in os.listdir( autoLibRoot ):
        if not src.endswith( '.cpp' ):
            continue
        if os.path.isfile( os.path.join( autoLibRoot, src ) ):
            result.append( src )
    return result

def PrintUsage( listAutoLib = None ):
    if listAutoLib is None:
        listAutoLib = GetAutoLibDirList()
    print( 'Usage: python3 build.py [<target auto-lib>]' )
    print( 'Target auto-lib list:' )
    for autoLib in listAutoLib:
        print( '\t%s' % autoLib )
    return

def RunCmd( cmd ):
    print( 'Execute: %s' % cmd )
    return os.system( cmd )

def BuildAutoLib( target ):
    print( 'Selected target auto-lib : %s' % target )
    os.chdir( os.path.join( os.getcwd(), '..', target ) )
    print( 'Current working directory: %s' % os.getcwd() )
    listSrc = GetSourceList()
    result = RunCmd( 'g++ -g -std=c++14 -o test %s' % ' '.join( listSrc ) )
    if result == 0:
        print( 'Build Success' )
        print( 'Auto Test' )
        print( '--------------------------------' )
        RunCmd( './test' )
    else:
        print( 'Build Fail' )
    return

if __name__ == '__main__':
    argc = len( sys.argv )

    if argc == 1:
        print( 'select target auto-lib' )
        listAutoLib = GetAutoLibDirList()
        PrintUsage( listAutoLib )
    elif argc == 2:
        targetAutoLib = sys.argv[1]
        listAutoLib = GetAutoLibDirList()
        if targetAutoLib not in listAutoLib:
            print( 'Error: target not exists.' )
            PrintUsage( listAutoLib )
        else:
            BuildAutoLib( targetAutoLib )
    else:
        PrintUsage()


