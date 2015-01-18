#!/bin/bash
URL_GIT=https://github.com/yhfudev/cpp-libucd.git

# work version for LT_INIT
VER_GIT=872e0950ccaced127fe91a49d19ebf6348b083f9
SHORTCOMMIT=${VER_GIT:0:7}

SHORTNAME=cpp-libucd
DN_PROJ=${SHORTNAME}-${SHORTCOMMIT}

FN_RPMSPEC=libucd.spec

sed -i "s|%global shortname .*$|%global shortname ${SHORTNAME}|"    ${FN_RPMSPEC}
sed -i "s|%global commit .*$|%global commit ${VER_GIT}|"               ${FN_RPMSPEC}
sed -i "s|%global shortcommit .*$|%global shortcommit %(c=%{commit}; echo \${c:0:7})|"   ${FN_RPMSPEC}
sed -i "s|Source0:.*$|Source0:        %{shortname}-%{shortcommit}.tar.gz|"  ${FN_RPMSPEC}
sed -i "s|%setup .*$|%setup -qn %{shortname}-%{shortcommit}|"       ${FN_RPMSPEC}
sed -i "s|Release:.*$|Release:        1.git%{shortcommit}%{?dist}|" ${FN_RPMSPEC}
sed -i "s|Version:.*$|Version:        1.0.0|" ${FN_RPMSPEC}

RPMNAME=$(grep Name: ${FN_RPMSPEC} | awk '{print $2}')
FILELIST=$(grep "Source[0-9]*:" "${FN_RPMSPEC}" | awk '{print $2}' | sed -e "s|%{name}|${RPMNAME}|g" | sed -e "s|%{shortname}|${SHORTNAME}|g" | sed -e "s|%{shortcommit}|${SHORTCOMMIT}|g" | sed -e "s|%{shortgit}|${SHORTCOMMIT}|g")
cp ${FILELIST} ~/rpmbuild/SOURCES/

#yum install -y python2-devel

if [ ! -f ${DN_PROJ} ]; then
  # clone
  echo git clone ${URL_GIT} ${DN_PROJ}
  git clone ${URL_GIT} ${DN_PROJ}
fi

cd ${DN_PROJ}

# clean the repo
#git stash
#git clean -dfx
#git rm --cached -r .
#git reset --soft $(git rev-list --max-parents=0 HEAD)
#git prune
#git pull
git checkout ${VER_GIT}

cd ..

rm -f ${DN_PROJ}.tar.gz
tar -cvzf ${DN_PROJ}.tar.gz ${DN_PROJ}

rpmdev-setuptree
cp ${DN_PROJ}.tar.gz ${FN_RPMSPEC} ~/rpmbuild/SOURCES/
rpmbuild -ba --clean ~/rpmbuild/SOURCES/${FN_RPMSPEC}

find ~/rpmbuild/RPMS/
find ~/rpmbuild/SRPMS/
