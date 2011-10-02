LINGUAS=ru
PACKAGE=vk-gtk
VERSION=0.3
PROJECT_ROOT=..

all:
	for ii in $(LINGUAS); do		\
	  mkdir -p $$ii;			\
	  msgfmt -o $$ii/$(PACKAGE).mo $$ii.po;	\
	done;

update-po:
	for ii in $(LINGUAS); do																				\
	  xgettext -j -s --package-name=$(PACKAGE) --package-version=$(VERSION) -o $$ii.po --keyword=_ --keyword=N_ --from-code=UTF-8 $(PROJECT_ROOT)/*/*.[hc]xx $(PROJECT_ROOT)/data/*.glade;	\
	done;
