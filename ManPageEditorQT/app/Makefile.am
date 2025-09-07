AUTOMAKE_OPTIONS = subdir-objects
include ../flagsandlibs
include ../sources

manpageeditorqt_SOURCES = $(COMMONSRC)

qtbasiceditor_CFLAGS = -O3
qtbasiceditor_CXXFLAGS = -O3

pixfilesdir = $(pkgdatadir)/pixmaps
pixfiles_DATA = ../resources/pixmaps/*

scriptfilesdir = $(pkgdatadir)/scripts
scriptfiles_SCRIPTS = ../resources/scripts/README

bin_PROGRAMS = manpageeditorqt

man1_MANS = ../resources/man/manpageeditorqt.1

install-exec-hook:
	install -D ../resources/pixmaps/ManPageEditorQT.png $(DESTDIR)${prefix}/share/pixmaps/ManPageEditorQT.png
	install -D ../resources/pixmaps/ManPageEditorQT.desktop $(DESTDIR)${prefix}/share/applications/ManPageEditorQT.desktop

uninstall-hook:
	rm -f $(DESTDIR)${prefix}/share/applications/ManPageEditorQT.desktop
	rm -f $(DESTDIR)${prefix}/share/pixmaps/ManPageEditorQT.png
	rm -fr $(pkgdatadir)

