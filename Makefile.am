SUBDIRS=src utils rpm pkgconfig langstats man
DISTCLEANFILES =

EXTRA_DIST=autogen.sh autoclean.sh Android.mk AUTHORS README.md ChangeLog

EXTRA_DIST += debian/* doc/* pacman/PKGBUILD test/*

CLEANFILES = *~ *.bz2 *.tar.gz *.log

#dist_doc_DATA = LICENSE
DISTCLEANFILES += *~ *.bz2 *.tar.gz *.log aclocal.m4 test/runtest.fail test/runtest.pass

# define rpm build root
RPMBUILDTOPDIR = $(shell unset RPMBUILDTOPDIR; if [ -f ${HOME}/.rpmmacros ]; then export RPMBUILDTOPDIR=`grep _topdir ${HOME}/.rpmmacros|awk '{print $$2}'`; else export RPMBUILDTOPDIR=/usr/src/redhat; fi; echo $${RPMBUILDTOPDIR};)

.PHONY: rpm

#${RPMBUILDTOPDIR}/rpmbuild/SOURCES/:
#	echo TEST: rpmdev-setuptree $@
#	$(shell echo "%_topdir ${HOME}/rpmbuild/" > ${HOME}/.rpmmacros)

rpm: dist rpm/@PACKAGE@.spec ${RPMBUILDTOPDIR}/SOURCES/ ${RPMBUILDTOPDIR}/SPECS/
	$(RM) ${RPMBUILDTOPDIR}/SOURCES/$(distdir).tar.gz
	$(RM) ${RPMBUILDTOPDIR}/SPECS/@PACKAGE@.spec
	cp $(srcdir)/$(distdir).tar.gz  ${RPMBUILDTOPDIR}/SOURCES/
	cp $(srcdir)/rpm/@PACKAGE@.spec ${RPMBUILDTOPDIR}/SPECS/
	rpmbuild -ba ${RPMBUILDTOPDIR}/SPECS/@PACKAGE@.spec

#	$(shell git log v2.1.0...v2.1.1 --pretty=format:'<li> <a href="http://github.com/yhfudev/cpp-libucd/commit/%H">view commit &bull;</a> %s</li> ' --reverse | grep "#changelog" > ChangeLog.html)
# git log --graph --pretty=format:'[%h] %cI %an: %s' --no-merges --abbrev-commit --date=iso
#git log --no-merges --date-order --date=short | \
#    sed -e '/^commit.*$/d' | \
#    awk '/^Author/ {sub(/\\$/,""); getline t; print $0 t; next}; 1' | \
#    sed -e 's/^Author: //g' | \
#    sed -e 's/>Date:   \([0-9]*-[0-9]*-[0-9]*\)/>\t\1/g' | \
#    sed -e 's/^\(.*\) \(\)\t\(.*\)/\3    \1    \2/g'
ChangeLog:
	if test -d $(srcdir)/.git; then                         \
	  if test -f $(srcdir)/.last-cl-gen; then               \
        git log --no-merges --date-order --date=short       \
          $$(cat $(srcdir)/.last-cl-gen)..                  \
          | sed -e  '/^commit.*$$/d'                        \
          | awk '/^Author/ {sub(/\\$$/,""); getline t; print $$0 t; next}; 1' \
          | sed -e 's/^Author: //g'                         \
          | sed -e 's/>Date:   \([0-9]*-[0-9]*-[0-9]*\)/>\t\1/g' \
          | sed -e 's/^\(.*\) \(\)\t\(.*\)/\3    \1    \2/g' \
          > ChangeLog.tmp;                                  \
      else                                                  \
        git log --no-merges --date-order --date=short       \
          | sed -e  '/^commit.*$$/d'                        \
          | awk '/^Author/ {sub(/\\$$/,""); getline t; print $$0 t; next}; 1' \
          | sed -e 's/^Author: //g'                         \
          | sed -e 's/>Date:   \([0-9]*-[0-9]*-[0-9]*\)/>\t\1/g' \
          | sed -e 's/^\(.*\) \(\)\t\(.*\)/\3    \1    \2/g' \
          > ChangeLog.tmp;                                  \
      fi;                                                   \
	  touch ChangeLog                                       \
	    && git rev-list -n 1 HEAD >.last-cl-gen.tmp         \
        && (echo; cat $(srcdir)/ChangeLog) >>ChangeLog.tmp  \
        && mv -f ChangeLog.tmp $(srcdir)/ChangeLog          \
        && mv -f .last-cl-gen.tmp $(srcdir)/.last-cl-gen    \
        && rm -f ChangeLog.tmp;                             \
    fi
	if test -d $(srcdir)/.hg; then                          \
        hg log --template changelog > ChangeLog;            \
        touch $(srcdir)/.last-cl-gen;                       \
    fi

EXTRA_DIST += .last-cl-gen
