# !/bin/bash

POSITIONAL=()
UPDATE=()
REQ_APPS=()
while [[ $# -gt 0 ]]; do
  key="$1"

  case $key in
	-n|--no-update)
	  UPDATE="$2"
	  shift # past argument
	  shift # past value
	  sudo apt install make gcc build-essential mtools qemu nasm
	  ;;
	-y|--update)
	  UPDATE="$2"
	  shift # past argument
	  shift # past value

	  sudo apt update
	  sudo apt upgrade
	  sudo apt install make gcc build-essential mtools qemu nasm
	  ;;
	*)    # unknown option
	  POSITIONAL+=("$1") # save it in an array for later
	  shift # past argument
	  ;;
  esac
done