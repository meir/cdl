function cdl {
	KEY="${1:=last}"
	cd $(cat ~/.cdlst | egrep '^'$KEY' (.*)' | sed 's/.* //g')
}

function cdp {
	KEY="${1:=last}"
	echo $(cat ~/.cdlst | egrep '^'$KEY' (.*)' | sed 's/.* //g')
}

function cds() {
	KEY="${1:=last}"
	grep -q '^'$KEY' .*$' ~/.cdlst && gsed -i 's+'$KEY' .*+'$KEY' '$(pwd)'+' ~/.cdlst || echo $KEY' '$(pwd) >>~/.cdlst
}

function cdr() {
	gsed -i 's+'$1' .*$++' ~/.cdlst
}
