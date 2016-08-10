#!/bin/bash

BASHRC=$HOME/.bashrc
VIMRC=$HOME/.vimrc
VIM_DIR=$HOME/.vim
VRAPPERRC=$HOME/.vrapperrc

BUNDLE_DIR=$HOME/.vim/bundle
VUNDLE_DIR=$BUNDLE_DIR/Vundle.vim

# Link .bashrc
if [[ -e $BASHRC ]]; then
	echo "${BASHRC} exists, skipping"
else
	echo "linking ${BASHRC}"
	ln -s `readlink -f bashrc` $BASHRC
fi

# Link .vimrc
if [[ -e $VIMRC ]]; then
	echo "${VIMRC} exists, skipping"
else
	echo "linking ${VIMRC}"
	ln -s `readlink -f vimrc` $VIMRC
fi

# Link .vim
if [[ -e $VIM_DIR ]]; then
	echo "${VIM_DIR} exists, skipping"
else
	echo "linking ${VIM_DIR}"
	ln -s `readlink -f vim` $VIM_DIR
fi

# Link .vrapperrc
if [[ -e $VRAPPERRC ]]; then
	echo "${VRAPPERRC} exists, skipping"
else
	echo "linking ${VRAPPERRC}"
	ln -s `readlink -f vrapperrc` $VRAPPERRC
fi

if [[ -e $BUNDLE_DIR ]]; then
  echo "${BUNDLE_DIR} exists, skipping"
else
  echo "creating ${BUNDLE_DIR}"
  mkdir $BUNDLE_DIR
fi

if [[ -e $VUNDLE_DIR ]]; then
  echo "${VUNDLE_DIR} exists, skipping"
else
  echo "cloning Vundle repository to ${VUNDLE_DIR}"
  git clone git://github.com/VundleVim/Vundle.vim.git ${VUNDLE_DIR}
fi

