cmds="cdls cdp cdr cds"

create_temp_dir() {
	temp_dir=$(mktemp -d 2>/dev/null || mktemp -d -t 'temp_dir')
	printf "Created temp dir: $temp_dir\n"
}

remove_temp_dir() {
	rm -rf $temp_dir
}

build() {
	mkdir -p bin/
	for cmd in $cmds; do
		echo "Building $cmd..."
		gcc -o bin/$cmd src/$cmd.c src/cache/cache.c
	done
}

install() {
	read -p "Do you want to install the commands globally? (/usr/local/bin) [y/N] " -n 1 -r
	printf "\n"

	if [[ $REPLY =~ ^[Yy]$ ]]; then
		mkdir -p /usr/local/bin
		for cmd in $(ls bin/); do
			echo "Installing $cmd..."
			cp bin/$cmd /usr/local/bin
		done
		export installed=1
	fi
}

uninstall() {
	read -p "Are you sure you want to delete the following commands from your system? $cmds [y/N] " -n 1 -r
	printf "\n"
	if [[ $REPLY =~ ^[Yy]$ ]]; then
		for cmd in $cmds; do
			echo "Uninstalling $cmd..."
			rm $(which $cmd)
		done
	fi
}

get_profile() {
	if [ -f ~/.zshrc ]; then
		echo ~/.zshrc
	elif [ -f ~/.bashrc ]; then
		echo ~/.bashrc
	elif [ -f ~/.bash_profile ]; then
		echo ~/.bash_profile
	elif [ -f ~/.profile ]; then
		echo ~/.profile
	else
		printf "Could not find a profile file. Please add the following line to your shell profile:\n"
		cat ./aliases
	fi
}

add_completion() {
	is_omz=$(command -v omz)
	if [[ -n "$is_omz" ]]; then
		read -p "Do you want to add completion to your shell? [y/N] " -n 1 -r
		if [[ $REPLY =~ ^[Yy]$ ]]; then
			mkdir -p ~/.oh-my-zsh/completions
			cp ./_cdl ~/.oh-my-zsh/completions/
		fi
	fi
}

case $1 in
build)
	build
	;;
uninstall)
	uninstall
	;;
*)
	create_temp_dir
	curr=$(pwd)
	cd $temp_dir

	git clone https://github.com/meir/cdl.git .
	build
	install

	if [[ $installed == 1 ]]; then
		profile=$(get_profile)
		exists=$(cat $profile | grep -c "cdls" | wc -l)
		if [[ $exists == 0 ]]; then
			read -p "Do you want to add the aliases to your shell profile? ($profile) [y/N] " -n 1 -r
			printf "\n"
			if [[ $REPLY =~ ^[Yy]$ ]]; then
				cat ./aliases >>$profile
				add_completion
			fi
		else
			read -p ""$profile" already contains aliases. Do you want to overwrite them? [y/N] " -n 1 -r
			printf "\n"
			if [[ $REPLY =~ ^[Yy]$ ]]; then
				# remove old alias without sed since sed works different between linux and darwin
				cp $profile ./profile
				grep -v -e "alias cdl" -e "cdl()" $profile >./profile
				mv ./profile $profile
				cat ./aliases >>$profile
				add_completion
			fi
		fi
	else
		printf "Cancelling installation...\n"
	fi

	cd $curr
	remove_temp_dir
	unset curr
	unset profile
	unset installed
	;;
esac
