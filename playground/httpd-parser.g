//From: https://github.com/reyk/httpd/blob/master/httpd/parse.y
httpd {

%whitespace "[ \t\r\n]*";
%whitespace "//[^\n\r]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

/*Tokens*/
//%token YYEOF
//%token YYerror
//%token YYUNDEF
//%token ACCESS
//%token ALIAS
//%token AUTO
//%token BACKLOG
//%token BODY
//%token BUFFER
//%token CERTIFICATE
//%token CHROOT
//%token CIPHERS
//%token COMMON
//%token COMBINED
//%token CONNECTION
//%token DHE
//%token DIRECTORY
//%token ECDHE
//%token ERR
//%token FCGI
//%token INDEX
//%token IP
//%token KEY
//%token LIFETIME
//%token LISTEN
//%token LOCATION
//%token LOG
//%token LOGDIR
//%token MATCH
//%token MAXIMUM
//%token NO
//%token NODELAY
//%token OCSP
//%token ON
//%token PORT
//%token PREFORK
//%token PROTOCOLS
//%token REQUESTS
//%token ROOT
//%token SACK
//%token SERVER
//%token SOCKET
//%token STRIP
//%token STYLE
//%token SYSLOG
//%token TCP
//%token TICKET
//%token TIMEOUT
//%token TLS
//%token TYPE
//%token TYPES
//%token HSTS
//%token MAXAGE
//%token SUBDOMAINS
//%token DEFAULT
//%token PRELOAD
//%token REQUEST
//%token ERROR
//%token INCLUDE
//%token AUTHENTICATE
//%token WITH
//%token BLOCK
//%token DROP
//%token RETURN
//%token PASS
//%token STRING
//%token NUMBER
//%token '\n'
//%token '='
//%token '{'
//%token '}'
//%token '/'
//%token ';'
//%token ','


//%start grammar

//%%

grammar :
	/*YYEOF
	|*/ /*empty*/
	| grammar include '\n'
	| grammar '\n'
	| grammar varset '\n'
	| grammar main '\n'
	| grammar server '\n'
	| grammar types '\n'
	| grammar error '\n'
	;

include :
	INCLUDE STRING
	;

varset :
	STRING '=' STRING
	;

opttls :
	/*empty*/
	| TLS
	;

main :
	PREFORK NUMBER
	| CHROOT STRING
	| LOGDIR STRING
	| DEFAULT TYPE mediastring
	;

server :
	SERVER optmatch STRING '{' optnl serveropts_l '}'
	;

serveropts_l :
	serveropts_l serveroptsl nl
	| serveroptsl optnl
	;

serveroptsl :
	LISTEN ON STRING opttls port
	| ALIAS optmatch STRING
	| tcpip
	| connection
	| tls
	| root
	| directory
	| logformat
	| fastcgi
	| authenticate
	| filter
	| LOCATION optmatch STRING '{' optnl serveropts_l '}'
	| DEFAULT TYPE mediastring
	| include
	| hsts
	;

hsts :
	HSTS '{' optnl hstsflags_l '}'
	| HSTS hstsflags
	| HSTS
	;

hstsflags_l :
	hstsflags optcommanl hstsflags_l
	| hstsflags optnl
	;

hstsflags :
	MAXAGE NUMBER
	| SUBDOMAINS
	| PRELOAD
	;

fastcgi :
	NO FCGI
	| FCGI
	| FCGI '{' optnl fcgiflags_l '}'
	| FCGI fcgiflags
	;

fcgiflags_l :
	fcgiflags optcommanl fcgiflags_l
	| fcgiflags optnl
	;

fcgiflags :
	SOCKET STRING
	;

connection :
	CONNECTION '{' optnl conflags_l '}'
	| CONNECTION conflags
	;

conflags_l :
	conflags optcommanl conflags_l
	| conflags optnl
	;

conflags :
	TIMEOUT timeout
	| REQUEST TIMEOUT timeout
	| MAXIMUM REQUESTS NUMBER
	| MAXIMUM REQUEST BODY NUMBER
	;

tls :
	TLS '{' optnl tlsopts_l '}'
	| TLS tlsopts
	;

tlsopts_l :
	tlsopts optcommanl tlsopts_l
	| tlsopts optnl
	;

tlsopts :
	CERTIFICATE STRING
	| KEY STRING
	| OCSP STRING
	| CIPHERS STRING
	| DHE STRING
	| ECDHE STRING
	| PROTOCOLS STRING
	| TICKET LIFETIME DEFAULT
	| TICKET LIFETIME NUMBER
	| NO TICKET
	;

root :
	ROOT rootflags
	| ROOT '{' optnl rootflags_l '}'
	;

rootflags_l :
	rootflags optcommanl rootflags_l
	| rootflags optnl
	;

rootflags :
	STRING
	| STRIP NUMBER
	;

authenticate :
	NO AUTHENTICATE
	| AUTHENTICATE authopts
	;

authopts :
	STRING WITH STRING
	| WITH STRING
	;

directory :
	DIRECTORY dirflags
	| DIRECTORY '{' optnl dirflags_l '}'
	;

dirflags_l :
	dirflags optcommanl dirflags_l
	| dirflags optnl
	;

dirflags :
	INDEX STRING
	| NO INDEX
	| AUTO INDEX
	| NO AUTO INDEX
	;

logformat :
	LOG logflags
	| LOG '{' optnl logflags_l '}'
	| NO LOG
	;

logflags_l :
	logflags optcommanl logflags_l
	| logflags optnl
	;

logflags :
	STYLE logstyle
	| SYSLOG
	| NO SYSLOG
	| ACCESS STRING
	| ERR STRING
	;

logstyle :
	COMMON
	| COMBINED
	| CONNECTION
	;

filter :
	block RETURN NUMBER optstring
	| block DROP
	| block
	| PASS
	;

block :
	BLOCK
	;

optmatch :
	/*empty*/
	| MATCH
	;

optstring :
	/*empty*/
	| STRING
	;

tcpip :
	TCP '{' optnl tcpflags_l '}'
	| TCP tcpflags
	;

tcpflags_l :
	tcpflags optcommanl tcpflags_l
	| tcpflags optnl
	;

tcpflags :
	SACK
	| NO SACK
	| NODELAY
	| NO NODELAY
	| BACKLOG NUMBER
	| SOCKET BUFFER NUMBER
	| IP STRING NUMBER
	;

types :
	TYPES '{' optnl mediaopts_l '}'
	;

mediaopts_l :
	mediaopts_l mediaoptsl nl
	| mediaoptsl nl
	;

mediaoptsl :
	mediastring medianames_l optsemicolon
	| include
	;

mediastring :
	STRING '/' STRING
	;

medianames_l :
	medianames_l medianamesl
	| medianamesl
	;

medianamesl :
	numberstring
	;

port :
	PORT NUMBER
	| PORT STRING
	;

timeout :
	NUMBER
	;

numberstring :
	NUMBER
	| STRING
	;

optsemicolon :
	';'
	| /*empty*/
	;

optnl :
	'\n' optnl
	| /*empty*/
	;

optcommanl :
	',' optnl
	| nl
	;

nl :
	'\n' optnl
	;

//Lexer

INCLUDE : 'include' ;
STRING : "'(\\.|[^\\'])+'" ;
TLS : 'tls' ;
PREFORK : 'prefork' ;
NUMBER : 'NUMBER' ;
CHROOT : 'chroot' ;
LOGDIR : 'logdir' ;
DEFAULT : 'default' ;
TYPE : 'type' ;
SERVER : 'server' ;
LISTEN : 'listen' ;
ON : 'on' ;
ALIAS : 'alias' ;
LOCATION : 'location' ;
HSTS : 'hsts' ;
MAXAGE : 'maxage' ;
SUBDOMAINS : 'subdomains' ;
PRELOAD : 'preload' ;
NO : 'no' ;
FCGI : 'fcgi' ;
SOCKET : 'socket' ;
CONNECTION : 'connection' ;
TIMEOUT : 'timeout' ;
REQUEST : 'request' ;
MAXIMUM : 'maximum' ;
REQUESTS : 'requests' ;
BODY : 'body' ;
CERTIFICATE : 'certificate' ;
KEY : 'key' ;
OCSP : 'ocsp' ;
CIPHERS : 'ciphers' ;
DHE : 'dhe' ;
ECDHE : 'ecdhe' ;
PROTOCOLS : 'protocols' ;
TICKET : 'ticket' ;
LIFETIME : 'lifetime' ;
ROOT : 'root' ;
STRIP : 'strip' ;
AUTHENTICATE : 'authenticate' ;
WITH : 'with' ;
DIRECTORY : 'directory' ;
INDEX : 'index' ;
AUTO : 'auto' ;
LOG : 'log' ;
STYLE : 'style' ;
SYSLOG : 'syslog' ;
ACCESS : 'access' ;
ERR : 'err' ;
COMMON : 'common' ;
COMBINED : 'combined' ;
RETURN : 'return' ;
DROP : 'drop' ;
PASS : 'pass' ;
BLOCK : 'block' ;
MATCH : 'match' ;
TCP : 'tcp' ;
SACK : 'sack' ;
NODELAY : 'nodelay' ;
BACKLOG : 'backlog' ;
BUFFER : 'buffer' ;
IP : 'ip' ;
TYPES : 'types' ;
PORT : 'port' ;

}
