C GEM library
=============


Get sources. GitHub HOWTO
=========================

Get sources from GitHub
-----------------------

```
cd ~/prj/github
mkdir -p cgem/cgem
cd cgem/cgem
git init
```

```
git remote add origin https://github.com/curoles/cgem.git
# Creates a remote named "origin" pointing at your GitHub repository
```

```
git pull origin master
```

Commit changes
--------------

```
git add ...
git commit -a -m "comments" <files>
git push
```

Build
=====

Install GNU Make 4.x
--------------------
New version >= 4.0 of GNU Make is required to build CGEM.
Install it if you do not have it.
```
cd ~/tools/
mkdir make4
cd make4/
wget http://ftp.gnu.org/gnu/make/make-4.1.tar.gz
tar -xzf make-4.1.tar.gz
cd make-4.1/
mkdir install
./configure --prefix=`pwd`/install
make
make install
install/bin/make --version
```

Generating build infra
----------------------
- Create a build directory:
```
cd ~/prj/github/cgem
mkdir build
```
- cd into the build directory
```
cd build
```
- Generate build infrastructure by calling:
```
make -f <cgem source path>/MakeBuild.makefile)
```
for example:
```
~/tools/make4/make-4.1/install/bin/make -f ../cgem/MakeBuild.makefile
```
- Call `make` to build CGEM.
  Call `make debug_make=1` to see noisy print-outs. 


New version >= 4.0 of GNU Make is required to build CGEM.
If you see message like `*** Required make version is 4.0.  Stop.`,
then you have to use newer version of make, see above how to install GNU Make 4.x.


