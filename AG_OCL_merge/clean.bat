del *.suo /Q
del *.suo /AH /Q
del *.sdf /Q
del *.db /Q
rd debug /S /Q
rd release /S /Q
del x64\Debug\*.* /S /Q
del x64\Release\*.* /S /Q
del x64\Debug/ocl_lab.tlog\*.* /S /Q
del x64\Release/ocl_lab.tlog\*.* /S /Q
rd x64\Debug\ocl_lab.tlog /S /Q
rd x64\Release\ocl_lab.tlog /S /Q
rd x64\Debug /S /Q
rd x64\Release /S /Q
rd x64 /S /Q
rd ipch /S /Q
del *.exp
del *.lib
del *.user
del ocllab32*.ilk
del ocllab32*.pdb
del ocllab32*.exe
del ocllab64*.ilk
del ocllab64*.pdb
del ocllab64*.exe
del *.bsc