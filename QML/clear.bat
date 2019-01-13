@echo off 
set work_path="%cd%"

cd %work_path% 
for /R %%s in (*.qmlc) do ( 
echo %%s 
del %%s
)

for /R %%s in (*.jsC) do ( 
echo %%s 
del %%s
)
 
pause 