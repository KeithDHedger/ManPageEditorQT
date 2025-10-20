AUTOMAKE_OPTIONS = subdir-objects
include ../flagsandlibs
include ../sources

manpageeditorqt_SOURCES = $(COMMONSRC)

pixfilesdir = $(pkgdatadir)/pixmaps
pixfiles_DATA = ../resources/pixmaps/*

helpfilesdir = $(pkgdatadir)/help
helpfiles_DATA = ../resources/help/*

docfilesdir = $(pkgdatadir)/docs
docfiles_DATA = ../resources/docs/*

bin_PROGRAMS = manpageeditorqt

install-exec-hook:
	install -D ../resources/pixmaps/ManPageEditorQT.png $(DESTDIR)${prefix}/share/pixmaps/ManPageEditorQT.png
	install -D ../resources/pixmaps/ManPageEditorQT.desktop $(DESTDIR)${prefix}/share/applications/ManPageEditorQT.desktop

uninstall-hook:
	rm -f $(DESTDIR)${prefix}/share/applications/ManPageEditorQT.desktop
	rm -f $(DESTDIR)${prefix}/share/pixmaps/ManPageEditorQT.png
	rm -fr $(pkgdatadir)

