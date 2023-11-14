/*
/usr/bin/time /home/mingo/dev/c/A_grammars/lalr/lalr-nb/dist/Release/GNU-Linux/lalr-nb postgresql.g -p

/usr/bin/time /home/mingo/dev/c/A_grammars/lalr/lalr-nb/dist/Release/GNU-Linux/lalr-nb postgresql.g > /dev/null

32 bits
64.89user 0.54system 1:08.52elapsed 95%CPU (0avgtext+0avgdata 1076272maxresident)k
0inputs+0outputs (0major+268598minor)pagefaults 0swaps

64 bits
66.48user 0.75system 1:07.25elapsed 99%CPU (0avgtext+0avgdata 1621820maxresident)k
0inputs+0outputs (0major+404808minor)pagefaults 0swaps
*/

postgresql {

%whitespace "[ \t\r\n]*";
%whitespace "\-\-[^\n]*";
//%whitespace "/\*[^*]+\*/";
%whitespace "/\*:C_MultilineComment:";

%case_insensitive ;

/*Tokens*/
//%token YYEOF
//%token YYerror
//%token YYUNDEF
//%token IDENT
//%token UIDENT
//%token FCONST
//%token SCONST
//%token USCONST
//%token BCONST
//%token XCONST
//%token Op
//%token ICONST
//%token PARAM
//%token TYPECAST
//%token DOT_DOT
//%token COLON_EQUALS
//%token EQUALS_GREATER
//%token LESS_EQUALS
//%token GREATER_EQUALS
//%token NOT_EQUALS
//%token ABORT_P
//%token ABSENT
//%token ABSOLUTE_P
//%token ACCESS
//%token ACTION
//%token ADD_P
//%token ADMIN
//%token AFTER
//%token AGGREGATE
//%token ALL
//%token ALSO
//%token ALTER
//%token ALWAYS
//%token ANALYSE
//%token ANALYZE
//%token AND
//%token ANY
//%token ARRAY
//%token AS
//%token ASC
//%token ASENSITIVE
//%token ASSERTION
//%token ASSIGNMENT
//%token ASYMMETRIC
//%token ATOMIC
//%token AT
//%token ATTACH
//%token ATTRIBUTE
//%token AUTHORIZATION
//%token BACKWARD
//%token BEFORE
//%token BEGIN_P
//%token BETWEEN
//%token BIGINT
//%token BINARY
//%token BIT
//%token BOOLEAN_P
//%token BOTH
//%token BREADTH
//%token BY
//%token CACHE
//%token CALL
//%token CALLED
//%token CASCADE
//%token CASCADED
//%token CASE
//%token CAST
//%token CATALOG_P
//%token CHAIN
//%token CHAR_P
//%token CHARACTER
//%token CHARACTERISTICS
//%token CHECK
//%token CHECKPOINT
//%token CLASS
//%token CLOSE
//%token CLUSTER
//%token COALESCE
//%token COLLATE
//%token COLLATION
//%token COLUMN
//%token COLUMNS
//%token COMMENT
//%token COMMENTS
//%token COMMIT
//%token COMMITTED
//%token COMPRESSION
//%token CONCURRENTLY
//%token CONFIGURATION
//%token CONFLICT
//%token CONNECTION
//%token CONSTRAINT
//%token CONSTRAINTS
//%token CONTENT_P
//%token CONTINUE_P
//%token CONVERSION_P
//%token COPY
//%token COST
//%token CREATE
//%token CROSS
//%token CSV
//%token CUBE
//%token CURRENT_P
//%token CURRENT_CATALOG
//%token CURRENT_DATE
//%token CURRENT_ROLE
//%token CURRENT_SCHEMA
//%token CURRENT_TIME
//%token CURRENT_TIMESTAMP
//%token CURRENT_USER
//%token CURSOR
//%token CYCLE
//%token DATA_P
//%token DATABASE
//%token DAY_P
//%token DEALLOCATE
//%token DEC
//%token DECIMAL_P
//%token DECLARE
//%token DEFAULT
//%token DEFAULTS
//%token DEFERRABLE
//%token DEFERRED
//%token DEFINER
//%token DELETE_P
//%token DELIMITER
//%token DELIMITERS
//%token DEPENDS
//%token DEPTH
//%token DESC
//%token DETACH
//%token DICTIONARY
//%token DISABLE_P
//%token DISCARD
//%token DISTINCT
//%token DO
//%token DOCUMENT_P
//%token DOMAIN_P
//%token DOUBLE_P
//%token DROP
//%token EACH
//%token ELSE
//%token ENABLE_P
//%token ENCODING
//%token ENCRYPTED
//%token END_P
//%token ENUM_P
//%token ESCAPE
//%token EVENT
//%token EXCEPT
//%token EXCLUDE
//%token EXCLUDING
//%token EXCLUSIVE
//%token EXECUTE
//%token EXISTS
//%token EXPLAIN
//%token EXPRESSION
//%token EXTENSION
//%token EXTERNAL
//%token EXTRACT
//%token FALSE_P
//%token FAMILY
//%token FETCH
//%token FILTER
//%token FINALIZE
//%token FIRST_P
//%token FLOAT_P
//%token FOLLOWING
//%token FOR
//%token FORCE
//%token FOREIGN
//%token FORMAT
//%token FORWARD
//%token FREEZE
//%token FROM
//%token FULL
//%token FUNCTION
//%token FUNCTIONS
//%token GENERATED
//%token GLOBAL
//%token GRANT
//%token GRANTED
//%token GREATEST
//%token GROUP_P
//%token GROUPING
//%token GROUPS
//%token HANDLER
//%token HAVING
//%token HEADER_P
//%token HOLD
//%token HOUR_P
//%token IDENTITY_P
//%token IF_P
//%token ILIKE
//%token IMMEDIATE
//%token IMMUTABLE
//%token IMPLICIT_P
//%token IMPORT_P
//%token IN_P
//%token INCLUDE
//%token INCLUDING
//%token INCREMENT
//%token INDENT
//%token INDEX
//%token INDEXES
//%token INHERIT
//%token INHERITS
//%token INITIALLY
//%token INLINE_P
//%token INNER_P
//%token INOUT
//%token INPUT_P
//%token INSENSITIVE
//%token INSERT
//%token INSTEAD
//%token INT_P
//%token INTEGER
//%token INTERSECT
//%token INTERVAL
//%token INTO
//%token INVOKER
//%token IS
//%token ISNULL
//%token ISOLATION
//%token JOIN
//%token JSON
//%token JSON_ARRAY
//%token JSON_ARRAYAGG
//%token JSON_OBJECT
//%token JSON_OBJECTAGG
//%token KEY
//%token KEYS
//%token LABEL
//%token LANGUAGE
//%token LARGE_P
//%token LAST_P
//%token LATERAL_P
//%token LEADING
//%token LEAKPROOF
//%token LEAST
//%token LEFT
//%token LEVEL
//%token LIKE
//%token LIMIT
//%token LISTEN
//%token LOAD
//%token LOCAL
//%token LOCALTIME
//%token LOCALTIMESTAMP
//%token LOCATION
//%token LOCK_P
//%token LOCKED
//%token LOGGED
//%token MAPPING
//%token MATCH
//%token MATCHED
//%token MATERIALIZED
//%token MAXVALUE
//%token MERGE
//%token METHOD
//%token MINUTE_P
//%token MINVALUE
//%token MODE
//%token MONTH_P
//%token MOVE
//%token NAME_P
//%token NAMES
//%token NATIONAL
//%token NATURAL
//%token NCHAR
//%token NEW
//%token NEXT
//%token NFC
//%token NFD
//%token NFKC
//%token NFKD
//%token NO
//%token NONE
//%token NORMALIZE
//%token NORMALIZED
//%token NOT
//%token NOTHING
//%token NOTIFY
//%token NOTNULL
//%token NOWAIT
//%token NULL_P
//%token NULLIF
//%token NULLS_P
//%token NUMERIC
//%token OBJECT_P
//%token OF
//%token OFF
//%token OFFSET
//%token OIDS
//%token OLD
//%token ON
//%token ONLY
//%token OPERATOR
//%token OPTION
//%token OPTIONS
//%token OR
//%token ORDER
//%token ORDINALITY
//%token OTHERS
//%token OUT_P
//%token OUTER_P
//%token OVER
//%token OVERLAPS
//%token OVERLAY
//%token OVERRIDING
//%token OWNED
//%token OWNER
//%token PARALLEL
//%token PARAMETER
//%token PARSER
//%token PARTIAL
//%token PARTITION
//%token PASSING
//%token PASSWORD
//%token PLACING
//%token PLANS
//%token POLICY
//%token POSITION
//%token PRECEDING
//%token PRECISION
//%token PRESERVE
//%token PREPARE
//%token PREPARED
//%token PRIMARY
//%token PRIOR
//%token PRIVILEGES
//%token PROCEDURAL
//%token PROCEDURE
//%token PROCEDURES
//%token PROGRAM
//%token PUBLICATION
//%token QUOTE
//%token RANGE
//%token READ
//%token REAL
//%token REASSIGN
//%token RECHECK
//%token RECURSIVE
//%token REF_P
//%token REFERENCES
//%token REFERENCING
//%token REFRESH
//%token REINDEX
//%token RELATIVE_P
//%token RELEASE
//%token RENAME
//%token REPEATABLE
//%token REPLACE
//%token REPLICA
//%token RESET
//%token RESTART
//%token RESTRICT
//%token RETURN
//%token RETURNING
//%token RETURNS
//%token REVOKE
//%token RIGHT
//%token ROLE
//%token ROLLBACK
//%token ROLLUP
//%token ROUTINE
//%token ROUTINES
//%token ROW
//%token ROWS
//%token RULE
//%token SAVEPOINT
//%token SCALAR
//%token SCHEMA
//%token SCHEMAS
//%token SCROLL
//%token SEARCH
//%token SECOND_P
//%token SECURITY
//%token SELECT
//%token SEQUENCE
//%token SEQUENCES
//%token SERIALIZABLE
//%token SERVER
//%token SESSION
//%token SESSION_USER
//%token SET
//%token SETS
//%token SETOF
//%token SHARE
//%token SHOW
//%token SIMILAR
//%token SIMPLE
//%token SKIP
//%token SMALLINT
//%token SNAPSHOT
//%token SOME
//%token SQL_P
//%token STABLE
//%token STANDALONE_P
//%token START
//%token STATEMENT
//%token STATISTICS
//%token STDIN
//%token STDOUT
//%token STORAGE
//%token STORED
//%token STRICT_P
//%token STRIP_P
//%token SUBSCRIPTION
//%token SUBSTRING
//%token SUPPORT
//%token SYMMETRIC
//%token SYSID
//%token SYSTEM_P
//%token SYSTEM_USER
//%token TABLE
//%token TABLES
//%token TABLESAMPLE
//%token TABLESPACE
//%token TEMP
//%token TEMPLATE
//%token TEMPORARY
//%token TEXT_P
//%token THEN
//%token TIES
//%token TIME
//%token TIMESTAMP
//%token TO
//%token TRAILING
//%token TRANSACTION
//%token TRANSFORM
//%token TREAT
//%token TRIGGER
//%token TRIM
//%token TRUE_P
//%token TRUNCATE
//%token TRUSTED
//%token TYPE_P
//%token TYPES_P
//%token UESCAPE
//%token UNBOUNDED
//%token UNCOMMITTED
//%token UNENCRYPTED
//%token UNION
//%token UNIQUE
//%token UNKNOWN
//%token UNLISTEN
//%token UNLOGGED
//%token UNTIL
//%token UPDATE
//%token USER
//%token USING
//%token VACUUM
//%token VALID
//%token VALIDATE
//%token VALIDATOR
//%token VALUE_P
//%token VALUES
//%token VARCHAR
//%token VARIADIC
//%token VARYING
//%token VERBOSE
//%token VERSION_P
//%token VIEW
//%token VIEWS
//%token VOLATILE
//%token WHEN
//%token WHERE
//%token WHITESPACE_P
//%token WINDOW
//%token WITH
//%token WITHIN
//%token WITHOUT
//%token WORK
//%token WRAPPER
//%token WRITE
//%token XML_P
//%token XMLATTRIBUTES
//%token XMLCONCAT
//%token XMLELEMENT
//%token XMLEXISTS
//%token XMLFOREST
//%token XMLNAMESPACES
//%token XMLPARSE
//%token XMLPI
//%token XMLROOT
//%token XMLSERIALIZE
//%token XMLTABLE
//%token YEAR_P
//%token YES_P
//%token ZONE
//%token FORMAT_LA
//%token NOT_LA
//%token NULLS_LA
//%token WITH_LA
//%token WITHOUT_LA
//%token MODE_TYPE_NAME
//%token MODE_PLPGSQL_EXPR
//%token MODE_PLPGSQL_ASSIGN1
//%token MODE_PLPGSQL_ASSIGN2
//%token MODE_PLPGSQL_ASSIGN3
//%token '<'
//%token '>'
//%token '='
//%token '+'
//%token '-'
//%token '*'
//%token '/'
//%token '%'
//%token '^'
//%token UMINUS
//%token '['
//%token ']'
//%token '('
//%token ')'
//%token '.'
//%token ';'
//%token ','
//%token ':'

%nonassoc /*1*/ SET ;
%left /*2*/ EXCEPT UNION ;
%left /*3*/ INTERSECT ;
%left /*4*/ OR ;
%left /*5*/ AND ;
%right /*6*/ NOT ;
%nonassoc /*7*/ IS ISNULL NOTNULL ;
%nonassoc /*8*/ LESS_EQUALS GREATER_EQUALS NOT_EQUALS '<' '>' '=' ;
%nonassoc /*9*/ BETWEEN ILIKE IN_P LIKE SIMILAR NOT_LA ;
%nonassoc /*10*/ ESCAPE ;
%nonassoc /*11*/ JSON UNIQUE ;
%nonassoc /*12*/ KEYS OBJECT_P SCALAR VALUE_P ;
%nonassoc /*13*/ WITH WITHOUT ;
%nonassoc /*14*/ UNBOUNDED ;
%nonassoc /*15*/ IDENT CUBE FOLLOWING GROUPS PARTITION PRECEDING RANGE ROLLUP ROWS ;
%left /*16*/ Op OPERATOR ;
%left /*17*/ '+' '-' ;
%left /*18*/ '*' '/' '%' ;
%left /*19*/ '^' ;
%left /*20*/ AT ;
%left /*21*/ COLLATE ;
%right /*22*/ UMINUS ;
%left /*23*/ '[' ']' ;
%left /*24*/ '(' ')' ;
%left /*25*/ TYPECAST ;
%left /*26*/ '.' ;
%left /*27*/ CROSS FULL INNER_P JOIN LEFT NATURAL RIGHT ;

//%start parse_toplevel

//%%

parse_toplevel :
	/*YYEOF
	|*/ stmtmulti
	| MODE_TYPE_NAME Typename
	| MODE_PLPGSQL_EXPR PLpgSQL_Expr
	| MODE_PLPGSQL_ASSIGN1 PLAssignStmt
	| MODE_PLPGSQL_ASSIGN2 PLAssignStmt
	| MODE_PLPGSQL_ASSIGN3 PLAssignStmt
	;

stmtmulti :
	stmtmulti ';' toplevel_stmt
	| toplevel_stmt
	;

toplevel_stmt :
	stmt
	| TransactionStmtLegacy
	;

stmt :
	AlterEventTrigStmt
	| AlterCollationStmt
	| AlterDatabaseStmt
	| AlterDatabaseSetStmt
	| AlterDefaultPrivilegesStmt
	| AlterDomainStmt
	| AlterEnumStmt
	| AlterExtensionStmt
	| AlterExtensionContentsStmt
	| AlterFdwStmt
	| AlterForeignServerStmt
	| AlterFunctionStmt
	| AlterGroupStmt
	| AlterObjectDependsStmt
	| AlterObjectSchemaStmt
	| AlterOwnerStmt
	| AlterOperatorStmt
	| AlterTypeStmt
	| AlterPolicyStmt
	| AlterSeqStmt
	| AlterSystemStmt
	| AlterTableStmt
	| AlterTblSpcStmt
	| AlterCompositeTypeStmt
	| AlterPublicationStmt
	| AlterRoleSetStmt
	| AlterRoleStmt
	| AlterSubscriptionStmt
	| AlterStatsStmt
	| AlterTSConfigurationStmt
	| AlterTSDictionaryStmt
	| AlterUserMappingStmt
	| AnalyzeStmt
	| CallStmt
	| CheckPointStmt
	| ClosePortalStmt
	| ClusterStmt
	| CommentStmt
	| ConstraintsSetStmt
	| CopyStmt
	| CreateAmStmt
	| CreateAsStmt
	| CreateAssertionStmt
	| CreateCastStmt
	| CreateConversionStmt
	| CreateDomainStmt
	| CreateExtensionStmt
	| CreateFdwStmt
	| CreateForeignServerStmt
	| CreateForeignTableStmt
	| CreateFunctionStmt
	| CreateGroupStmt
	| CreateMatViewStmt
	| CreateOpClassStmt
	| CreateOpFamilyStmt
	| CreatePublicationStmt
	| AlterOpFamilyStmt
	| CreatePolicyStmt
	| CreatePLangStmt
	| CreateSchemaStmt
	| CreateSeqStmt
	| CreateStmt
	| CreateSubscriptionStmt
	| CreateStatsStmt
	| CreateTableSpaceStmt
	| CreateTransformStmt
	| CreateTrigStmt
	| CreateEventTrigStmt
	| CreateRoleStmt
	| CreateUserStmt
	| CreateUserMappingStmt
	| CreatedbStmt
	| DeallocateStmt
	| DeclareCursorStmt
	| DefineStmt
	| DeleteStmt
	| DiscardStmt
	| DoStmt
	| DropCastStmt
	| DropOpClassStmt
	| DropOpFamilyStmt
	| DropOwnedStmt
	| DropStmt
	| DropSubscriptionStmt
	| DropTableSpaceStmt
	| DropTransformStmt
	| DropRoleStmt
	| DropUserMappingStmt
	| DropdbStmt
	| ExecuteStmt
	| ExplainStmt
	| FetchStmt
	| GrantStmt
	| GrantRoleStmt
	| ImportForeignSchemaStmt
	| IndexStmt
	| InsertStmt
	| ListenStmt
	| RefreshMatViewStmt
	| LoadStmt
	| LockStmt
	| MergeStmt
	| NotifyStmt
	| PrepareStmt
	| ReassignOwnedStmt
	| ReindexStmt
	| RemoveAggrStmt
	| RemoveFuncStmt
	| RemoveOperStmt
	| RenameStmt
	| RevokeStmt
	| RevokeRoleStmt
	| RuleStmt
	| SecLabelStmt
	| SelectStmt
	| TransactionStmt
	| TruncateStmt
	| UnlistenStmt
	| UpdateStmt
	| VacuumStmt
	| VariableResetStmt
	| VariableSetStmt
	| VariableShowStmt
	| ViewStmt
	| /*empty*/
	;

opt_single_name :
	ColId
	| /*empty*/
	;

opt_qualified_name :
	any_name
	| /*empty*/
	;

opt_concurrently :
	CONCURRENTLY
	| /*empty*/
	;

opt_drop_behavior :
	CASCADE
	| RESTRICT
	| /*empty*/
	;

CallStmt :
	CALL func_application
	;

CreateRoleStmt :
	CREATE ROLE RoleId opt_with OptRoleList
	;

opt_with :
	WITH /*13N*/
	| WITH_LA
	| /*empty*/
	;

OptRoleList :
	OptRoleList CreateOptRoleElem
	| /*empty*/
	;

AlterOptRoleList :
	AlterOptRoleList AlterOptRoleElem
	| /*empty*/
	;

AlterOptRoleElem :
	PASSWORD Sconst
	| PASSWORD NULL_P
	| ENCRYPTED PASSWORD Sconst
	| UNENCRYPTED PASSWORD Sconst
	| INHERIT
	| CONNECTION LIMIT SignedIconst
	| VALID UNTIL Sconst
	| USER role_list
	| IDENT /*15N*/
	;

CreateOptRoleElem :
	AlterOptRoleElem
	| SYSID Iconst
	| ADMIN role_list
	| ROLE role_list
	| IN_P /*9N*/ ROLE role_list
	| IN_P /*9N*/ GROUP_P role_list
	;

CreateUserStmt :
	CREATE USER RoleId opt_with OptRoleList
	;

AlterRoleStmt :
	ALTER ROLE RoleSpec opt_with AlterOptRoleList
	| ALTER USER RoleSpec opt_with AlterOptRoleList
	;

opt_in_database :
	/*empty*/
	| IN_P /*9N*/ DATABASE name
	;

AlterRoleSetStmt :
	ALTER ROLE RoleSpec opt_in_database SetResetClause
	| ALTER ROLE ALL opt_in_database SetResetClause
	| ALTER USER RoleSpec opt_in_database SetResetClause
	| ALTER USER ALL opt_in_database SetResetClause
	;

DropRoleStmt :
	DROP ROLE role_list
	| DROP ROLE IF_P EXISTS role_list
	| DROP USER role_list
	| DROP USER IF_P EXISTS role_list
	| DROP GROUP_P role_list
	| DROP GROUP_P IF_P EXISTS role_list
	;

CreateGroupStmt :
	CREATE GROUP_P RoleId opt_with OptRoleList
	;

AlterGroupStmt :
	ALTER GROUP_P RoleSpec add_drop USER role_list
	;

add_drop :
	ADD_P
	| DROP
	;

CreateSchemaStmt :
	CREATE SCHEMA opt_single_name AUTHORIZATION RoleSpec OptSchemaEltList
	| CREATE SCHEMA ColId OptSchemaEltList
	| CREATE SCHEMA IF_P NOT /*6R*/ EXISTS opt_single_name AUTHORIZATION RoleSpec OptSchemaEltList
	| CREATE SCHEMA IF_P NOT /*6R*/ EXISTS ColId OptSchemaEltList
	;

OptSchemaEltList :
	OptSchemaEltList schema_stmt
	| /*empty*/
	;

schema_stmt :
	CreateStmt
	| IndexStmt
	| CreateSeqStmt
	| CreateTrigStmt
	| GrantStmt
	| ViewStmt
	;

VariableSetStmt :
	SET /*1N*/ set_rest
	| SET /*1N*/ LOCAL set_rest
	| SET /*1N*/ SESSION set_rest
	;

set_rest :
	TRANSACTION transaction_mode_list
	| SESSION CHARACTERISTICS AS TRANSACTION transaction_mode_list
	| set_rest_more
	;

generic_set :
	var_name TO var_list
	| var_name '=' /*8N*/ var_list
	| var_name TO DEFAULT
	| var_name '=' /*8N*/ DEFAULT
	;

set_rest_more :
	generic_set
	| var_name FROM CURRENT_P
	| TIME ZONE zone_value
	| CATALOG_P Sconst
	| SCHEMA Sconst
	| NAMES opt_encoding
	| ROLE NonReservedWord_or_Sconst
	| SESSION AUTHORIZATION NonReservedWord_or_Sconst
	| SESSION AUTHORIZATION DEFAULT
	| XML_P OPTION document_or_content
	| TRANSACTION SNAPSHOT Sconst
	;

var_name :
	ColId
	| var_name '.' /*26L*/ ColId
	;

var_list :
	var_value
	| var_list ',' var_value
	;

var_value :
	opt_boolean_or_string
	| NumericOnly
	;

iso_level :
	READ UNCOMMITTED
	| READ COMMITTED
	| REPEATABLE READ
	| SERIALIZABLE
	;

opt_boolean_or_string :
	TRUE_P
	| FALSE_P
	| ON
	| NonReservedWord_or_Sconst
	;

zone_value :
	Sconst
	| IDENT /*15N*/
	| ConstInterval Sconst opt_interval
	| ConstInterval '(' /*24L*/ Iconst ')' /*24L*/ Sconst
	| NumericOnly
	| DEFAULT
	| LOCAL
	;

opt_encoding :
	Sconst
	| DEFAULT
	| /*empty*/
	;

NonReservedWord_or_Sconst :
	NonReservedWord
	| Sconst
	;

VariableResetStmt :
	RESET reset_rest
	;

reset_rest :
	generic_reset
	| TIME ZONE
	| TRANSACTION ISOLATION LEVEL
	| SESSION AUTHORIZATION
	;

generic_reset :
	var_name
	| ALL
	;

SetResetClause :
	SET /*1N*/ set_rest
	| VariableResetStmt
	;

FunctionSetResetClause :
	SET /*1N*/ set_rest_more
	| VariableResetStmt
	;

VariableShowStmt :
	SHOW var_name
	| SHOW TIME ZONE
	| SHOW TRANSACTION ISOLATION LEVEL
	| SHOW SESSION AUTHORIZATION
	| SHOW ALL
	;

ConstraintsSetStmt :
	SET /*1N*/ CONSTRAINTS constraints_set_list constraints_set_mode
	;

constraints_set_list :
	ALL
	| qualified_name_list
	;

constraints_set_mode :
	DEFERRED
	| IMMEDIATE
	;

CheckPointStmt :
	CHECKPOINT
	;

DiscardStmt :
	DISCARD ALL
	| DISCARD TEMP
	| DISCARD TEMPORARY
	| DISCARD PLANS
	| DISCARD SEQUENCES
	;

AlterTableStmt :
	ALTER TABLE relation_expr alter_table_cmds
	| ALTER TABLE IF_P EXISTS relation_expr alter_table_cmds
	| ALTER TABLE relation_expr partition_cmd
	| ALTER TABLE IF_P EXISTS relation_expr partition_cmd
	| ALTER TABLE ALL IN_P /*9N*/ TABLESPACE name SET /*1N*/ TABLESPACE name opt_nowait
	| ALTER TABLE ALL IN_P /*9N*/ TABLESPACE name OWNED BY role_list SET /*1N*/ TABLESPACE name opt_nowait
	| ALTER INDEX qualified_name alter_table_cmds
	| ALTER INDEX IF_P EXISTS qualified_name alter_table_cmds
	| ALTER INDEX qualified_name index_partition_cmd
	| ALTER INDEX ALL IN_P /*9N*/ TABLESPACE name SET /*1N*/ TABLESPACE name opt_nowait
	| ALTER INDEX ALL IN_P /*9N*/ TABLESPACE name OWNED BY role_list SET /*1N*/ TABLESPACE name opt_nowait
	| ALTER SEQUENCE qualified_name alter_table_cmds
	| ALTER SEQUENCE IF_P EXISTS qualified_name alter_table_cmds
	| ALTER VIEW qualified_name alter_table_cmds
	| ALTER VIEW IF_P EXISTS qualified_name alter_table_cmds
	| ALTER MATERIALIZED VIEW qualified_name alter_table_cmds
	| ALTER MATERIALIZED VIEW IF_P EXISTS qualified_name alter_table_cmds
	| ALTER MATERIALIZED VIEW ALL IN_P /*9N*/ TABLESPACE name SET /*1N*/ TABLESPACE name opt_nowait
	| ALTER MATERIALIZED VIEW ALL IN_P /*9N*/ TABLESPACE name OWNED BY role_list SET /*1N*/ TABLESPACE name opt_nowait
	| ALTER FOREIGN TABLE relation_expr alter_table_cmds
	| ALTER FOREIGN TABLE IF_P EXISTS relation_expr alter_table_cmds
	;

alter_table_cmds :
	alter_table_cmd
	| alter_table_cmds ',' alter_table_cmd
	;

partition_cmd :
	ATTACH PARTITION /*15N*/ qualified_name PartitionBoundSpec
	| DETACH PARTITION /*15N*/ qualified_name opt_concurrently
	| DETACH PARTITION /*15N*/ qualified_name FINALIZE
	;

index_partition_cmd :
	ATTACH PARTITION /*15N*/ qualified_name
	;

alter_table_cmd :
	ADD_P columnDef
	| ADD_P IF_P NOT /*6R*/ EXISTS columnDef
	| ADD_P COLUMN columnDef
	| ADD_P COLUMN IF_P NOT /*6R*/ EXISTS columnDef
	| ALTER opt_column ColId alter_column_default
	| ALTER opt_column ColId DROP NOT /*6R*/ NULL_P
	| ALTER opt_column ColId SET /*1N*/ NOT /*6R*/ NULL_P
	| ALTER opt_column ColId DROP EXPRESSION
	| ALTER opt_column ColId DROP EXPRESSION IF_P EXISTS
	| ALTER opt_column ColId SET /*1N*/ STATISTICS SignedIconst
	| ALTER opt_column Iconst SET /*1N*/ STATISTICS SignedIconst
	| ALTER opt_column ColId SET /*1N*/ reloptions
	| ALTER opt_column ColId RESET reloptions
	| ALTER opt_column ColId SET /*1N*/ column_storage
	| ALTER opt_column ColId SET /*1N*/ column_compression
	| ALTER opt_column ColId ADD_P GENERATED generated_when AS IDENTITY_P OptParenthesizedSeqOptList
	| ALTER opt_column ColId alter_identity_column_option_list
	| ALTER opt_column ColId DROP IDENTITY_P
	| ALTER opt_column ColId DROP IDENTITY_P IF_P EXISTS
	| DROP opt_column IF_P EXISTS ColId opt_drop_behavior
	| DROP opt_column ColId opt_drop_behavior
	| ALTER opt_column ColId opt_set_data TYPE_P Typename opt_collate_clause alter_using
	| ALTER opt_column ColId alter_generic_options
	| ADD_P TableConstraint
	| ALTER CONSTRAINT name ConstraintAttributeSpec
	| VALIDATE CONSTRAINT name
	| DROP CONSTRAINT IF_P EXISTS name opt_drop_behavior
	| DROP CONSTRAINT name opt_drop_behavior
	| SET /*1N*/ WITHOUT /*13N*/ OIDS
	| CLUSTER ON name
	| SET /*1N*/ WITHOUT /*13N*/ CLUSTER
	| SET /*1N*/ LOGGED
	| SET /*1N*/ UNLOGGED
	| ENABLE_P TRIGGER name
	| ENABLE_P ALWAYS TRIGGER name
	| ENABLE_P REPLICA TRIGGER name
	| ENABLE_P TRIGGER ALL
	| ENABLE_P TRIGGER USER
	| DISABLE_P TRIGGER name
	| DISABLE_P TRIGGER ALL
	| DISABLE_P TRIGGER USER
	| ENABLE_P RULE name
	| ENABLE_P ALWAYS RULE name
	| ENABLE_P REPLICA RULE name
	| DISABLE_P RULE name
	| INHERIT qualified_name
	| NO INHERIT qualified_name
	| OF any_name
	| NOT /*6R*/ OF
	| OWNER TO RoleSpec
	| SET /*1N*/ ACCESS METHOD name
	| SET /*1N*/ TABLESPACE name
	| SET /*1N*/ reloptions
	| RESET reloptions
	| REPLICA IDENTITY_P replica_identity
	| ENABLE_P ROW LEVEL SECURITY
	| DISABLE_P ROW LEVEL SECURITY
	| FORCE ROW LEVEL SECURITY
	| NO FORCE ROW LEVEL SECURITY
	| alter_generic_options
	;

alter_column_default :
	SET /*1N*/ DEFAULT a_expr
	| DROP DEFAULT
	;

opt_collate_clause :
	COLLATE /*21L*/ any_name
	| /*empty*/
	;

alter_using :
	USING a_expr
	| /*empty*/
	;

replica_identity :
	NOTHING
	| FULL /*27L*/
	| DEFAULT
	| USING INDEX name
	;

reloptions :
	'(' /*24L*/ reloption_list ')' /*24L*/
	;

opt_reloptions :
	WITH /*13N*/ reloptions
	| /*empty*/
	;

reloption_list :
	reloption_elem
	| reloption_list ',' reloption_elem
	;

reloption_elem :
	ColLabel '=' /*8N*/ def_arg
	| ColLabel
	| ColLabel '.' /*26L*/ ColLabel '=' /*8N*/ def_arg
	| ColLabel '.' /*26L*/ ColLabel
	;

alter_identity_column_option_list :
	alter_identity_column_option
	| alter_identity_column_option_list alter_identity_column_option
	;

alter_identity_column_option :
	RESTART
	| RESTART opt_with NumericOnly
	| SET /*1N*/ SeqOptElem
	| SET /*1N*/ GENERATED generated_when
	;

PartitionBoundSpec :
	FOR VALUES WITH /*13N*/ '(' /*24L*/ hash_partbound ')' /*24L*/
	| FOR VALUES IN_P /*9N*/ '(' /*24L*/ expr_list ')' /*24L*/
	| FOR VALUES FROM '(' /*24L*/ expr_list ')' /*24L*/ TO '(' /*24L*/ expr_list ')' /*24L*/
	| DEFAULT
	;

hash_partbound_elem :
	NonReservedWord Iconst
	;

hash_partbound :
	hash_partbound_elem
	| hash_partbound ',' hash_partbound_elem
	;

AlterCompositeTypeStmt :
	ALTER TYPE_P any_name alter_type_cmds
	;

alter_type_cmds :
	alter_type_cmd
	| alter_type_cmds ',' alter_type_cmd
	;

alter_type_cmd :
	ADD_P ATTRIBUTE TableFuncElement opt_drop_behavior
	| DROP ATTRIBUTE IF_P EXISTS ColId opt_drop_behavior
	| DROP ATTRIBUTE ColId opt_drop_behavior
	| ALTER ATTRIBUTE ColId opt_set_data TYPE_P Typename opt_collate_clause opt_drop_behavior
	;

ClosePortalStmt :
	CLOSE cursor_name
	| CLOSE ALL
	;

CopyStmt :
	COPY opt_binary qualified_name opt_column_list copy_from opt_program copy_file_name copy_delimiter opt_with copy_options where_clause
	| COPY '(' /*24L*/ PreparableStmt ')' /*24L*/ TO opt_program copy_file_name opt_with copy_options
	;

copy_from :
	FROM
	| TO
	;

opt_program :
	PROGRAM
	| /*empty*/
	;

copy_file_name :
	Sconst
	| STDIN
	| STDOUT
	;

copy_options :
	copy_opt_list
	| '(' /*24L*/ copy_generic_opt_list ')' /*24L*/
	;

copy_opt_list :
	copy_opt_list copy_opt_item
	| /*empty*/
	;

copy_opt_item :
	BINARY
	| FREEZE
	| DELIMITER opt_as Sconst
	| NULL_P opt_as Sconst
	| CSV
	| HEADER_P
	| QUOTE opt_as Sconst
	| ESCAPE /*10N*/ opt_as Sconst
	| FORCE QUOTE columnList
	| FORCE QUOTE '*' /*18L*/
	| FORCE NOT /*6R*/ NULL_P columnList
	| FORCE NULL_P columnList
	| ENCODING Sconst
	;

opt_binary :
	BINARY
	| /*empty*/
	;

copy_delimiter :
	opt_using DELIMITERS Sconst
	| /*empty*/
	;

opt_using :
	USING
	| /*empty*/
	;

copy_generic_opt_list :
	copy_generic_opt_elem
	| copy_generic_opt_list ',' copy_generic_opt_elem
	;

copy_generic_opt_elem :
	ColLabel copy_generic_opt_arg
	;

copy_generic_opt_arg :
	opt_boolean_or_string
	| NumericOnly
	| '*' /*18L*/
	| '(' /*24L*/ copy_generic_opt_arg_list ')' /*24L*/
	| /*empty*/
	;

copy_generic_opt_arg_list :
	copy_generic_opt_arg_list_item
	| copy_generic_opt_arg_list ',' copy_generic_opt_arg_list_item
	;

copy_generic_opt_arg_list_item :
	opt_boolean_or_string
	;

CreateStmt :
	CREATE OptTemp TABLE qualified_name '(' /*24L*/ OptTableElementList ')' /*24L*/ OptInherit OptPartitionSpec table_access_method_clause OptWith OnCommitOption OptTableSpace
	| CREATE OptTemp TABLE IF_P NOT /*6R*/ EXISTS qualified_name '(' /*24L*/ OptTableElementList ')' /*24L*/ OptInherit OptPartitionSpec table_access_method_clause OptWith OnCommitOption OptTableSpace
	| CREATE OptTemp TABLE qualified_name OF any_name OptTypedTableElementList OptPartitionSpec table_access_method_clause OptWith OnCommitOption OptTableSpace
	| CREATE OptTemp TABLE IF_P NOT /*6R*/ EXISTS qualified_name OF any_name OptTypedTableElementList OptPartitionSpec table_access_method_clause OptWith OnCommitOption OptTableSpace
	| CREATE OptTemp TABLE qualified_name PARTITION /*15N*/ OF qualified_name OptTypedTableElementList PartitionBoundSpec OptPartitionSpec table_access_method_clause OptWith OnCommitOption OptTableSpace
	| CREATE OptTemp TABLE IF_P NOT /*6R*/ EXISTS qualified_name PARTITION /*15N*/ OF qualified_name OptTypedTableElementList PartitionBoundSpec OptPartitionSpec table_access_method_clause OptWith OnCommitOption OptTableSpace
	;

OptTemp :
	TEMPORARY
	| TEMP
	| LOCAL TEMPORARY
	| LOCAL TEMP
	| GLOBAL TEMPORARY
	| GLOBAL TEMP
	| UNLOGGED
	| /*empty*/
	;

OptTableElementList :
	TableElementList
	| /*empty*/
	;

OptTypedTableElementList :
	'(' /*24L*/ TypedTableElementList ')' /*24L*/
	| /*empty*/
	;

TableElementList :
	TableElement
	| TableElementList ',' TableElement
	;

TypedTableElementList :
	TypedTableElement
	| TypedTableElementList ',' TypedTableElement
	;

TableElement :
	columnDef
	| TableLikeClause
	| TableConstraint
	;

TypedTableElement :
	columnOptions
	| TableConstraint
	;

columnDef :
	ColId Typename opt_column_storage opt_column_compression create_generic_options ColQualList
	;

columnOptions :
	ColId ColQualList
	| ColId WITH /*13N*/ OPTIONS ColQualList
	;

column_compression :
	COMPRESSION ColId
	| COMPRESSION DEFAULT
	;

opt_column_compression :
	column_compression
	| /*empty*/
	;

column_storage :
	STORAGE ColId
	| STORAGE DEFAULT
	;

opt_column_storage :
	column_storage
	| /*empty*/
	;

ColQualList :
	ColQualList ColConstraint
	| /*empty*/
	;

ColConstraint :
	CONSTRAINT name ColConstraintElem
	| ColConstraintElem
	| ConstraintAttr
	| COLLATE /*21L*/ any_name
	;

ColConstraintElem :
	NOT /*6R*/ NULL_P
	| NULL_P
	| UNIQUE /*11N*/ opt_unique_null_treatment opt_definition OptConsTableSpace
	| PRIMARY KEY opt_definition OptConsTableSpace
	| CHECK '(' /*24L*/ a_expr ')' /*24L*/ opt_no_inherit
	| DEFAULT b_expr
	| GENERATED generated_when AS IDENTITY_P OptParenthesizedSeqOptList
	| GENERATED generated_when AS '(' /*24L*/ a_expr ')' /*24L*/ STORED
	| REFERENCES qualified_name opt_column_list key_match key_actions
	;

opt_unique_null_treatment :
	NULLS_P DISTINCT
	| NULLS_P NOT /*6R*/ DISTINCT
	| /*empty*/
	;

generated_when :
	ALWAYS
	| BY DEFAULT
	;

ConstraintAttr :
	DEFERRABLE
	| NOT /*6R*/ DEFERRABLE
	| INITIALLY DEFERRED
	| INITIALLY IMMEDIATE
	;

TableLikeClause :
	LIKE /*9N*/ qualified_name TableLikeOptionList
	;

TableLikeOptionList :
	TableLikeOptionList INCLUDING TableLikeOption
	| TableLikeOptionList EXCLUDING TableLikeOption
	| /*empty*/
	;

TableLikeOption :
	COMMENTS
	| COMPRESSION
	| CONSTRAINTS
	| DEFAULTS
	| IDENTITY_P
	| GENERATED
	| INDEXES
	| STATISTICS
	| STORAGE
	| ALL
	;

TableConstraint :
	CONSTRAINT name ConstraintElem
	| ConstraintElem
	;

ConstraintElem :
	CHECK '(' /*24L*/ a_expr ')' /*24L*/ ConstraintAttributeSpec
	| UNIQUE /*11N*/ opt_unique_null_treatment '(' /*24L*/ columnList ')' /*24L*/ opt_c_include opt_definition OptConsTableSpace ConstraintAttributeSpec
	| UNIQUE /*11N*/ ExistingIndex ConstraintAttributeSpec
	| PRIMARY KEY '(' /*24L*/ columnList ')' /*24L*/ opt_c_include opt_definition OptConsTableSpace ConstraintAttributeSpec
	| PRIMARY KEY ExistingIndex ConstraintAttributeSpec
	| EXCLUDE access_method_clause '(' /*24L*/ ExclusionConstraintList ')' /*24L*/ opt_c_include opt_definition OptConsTableSpace OptWhereClause ConstraintAttributeSpec
	| FOREIGN KEY '(' /*24L*/ columnList ')' /*24L*/ REFERENCES qualified_name opt_column_list key_match key_actions ConstraintAttributeSpec
	;

opt_no_inherit :
	NO INHERIT
	| /*empty*/
	;

opt_column_list :
	'(' /*24L*/ columnList ')' /*24L*/
	| /*empty*/
	;

columnList :
	columnElem
	| columnList ',' columnElem
	;

columnElem :
	ColId
	;

opt_c_include :
	INCLUDE '(' /*24L*/ columnList ')' /*24L*/
	| /*empty*/
	;

key_match :
	MATCH FULL /*27L*/
	| MATCH PARTIAL
	| MATCH SIMPLE
	| /*empty*/
	;

ExclusionConstraintList :
	ExclusionConstraintElem
	| ExclusionConstraintList ',' ExclusionConstraintElem
	;

ExclusionConstraintElem :
	index_elem WITH /*13N*/ any_operator
	| index_elem WITH /*13N*/ OPERATOR /*16L*/ '(' /*24L*/ any_operator ')' /*24L*/
	;

OptWhereClause :
	WHERE '(' /*24L*/ a_expr ')' /*24L*/
	| /*empty*/
	;

key_actions :
	key_update
	| key_delete
	| key_update key_delete
	| key_delete key_update
	| /*empty*/
	;

key_update :
	ON UPDATE key_action
	;

key_delete :
	ON DELETE_P key_action
	;

key_action :
	NO ACTION
	| RESTRICT
	| CASCADE
	| SET /*1N*/ NULL_P opt_column_list
	| SET /*1N*/ DEFAULT opt_column_list
	;

OptInherit :
	INHERITS '(' /*24L*/ qualified_name_list ')' /*24L*/
	| /*empty*/
	;

OptPartitionSpec :
	PartitionSpec
	| /*empty*/
	;

PartitionSpec :
	PARTITION /*15N*/ BY ColId '(' /*24L*/ part_params ')' /*24L*/
	;

part_params :
	part_elem
	| part_params ',' part_elem
	;

part_elem :
	ColId opt_collate opt_qualified_name
	| func_expr_windowless opt_collate opt_qualified_name
	| '(' /*24L*/ a_expr ')' /*24L*/ opt_collate opt_qualified_name
	;

table_access_method_clause :
	USING name
	| /*empty*/
	;

OptWith :
	WITH /*13N*/ reloptions
	| WITHOUT /*13N*/ OIDS
	| /*empty*/
	;

OnCommitOption :
	ON COMMIT DROP
	| ON COMMIT DELETE_P ROWS /*15N*/
	| ON COMMIT PRESERVE ROWS /*15N*/
	| /*empty*/
	;

OptTableSpace :
	TABLESPACE name
	| /*empty*/
	;

OptConsTableSpace :
	USING INDEX TABLESPACE name
	| /*empty*/
	;

ExistingIndex :
	USING INDEX name
	;

CreateStatsStmt :
	CREATE STATISTICS opt_qualified_name opt_name_list ON stats_params FROM from_list
	| CREATE STATISTICS IF_P NOT /*6R*/ EXISTS any_name opt_name_list ON stats_params FROM from_list
	;

stats_params :
	stats_param
	| stats_params ',' stats_param
	;

stats_param :
	ColId
	| func_expr_windowless
	| '(' /*24L*/ a_expr ')' /*24L*/
	;

AlterStatsStmt :
	ALTER STATISTICS any_name SET /*1N*/ STATISTICS SignedIconst
	| ALTER STATISTICS IF_P EXISTS any_name SET /*1N*/ STATISTICS SignedIconst
	;

CreateAsStmt :
	CREATE OptTemp TABLE create_as_target AS SelectStmt opt_with_data
	| CREATE OptTemp TABLE IF_P NOT /*6R*/ EXISTS create_as_target AS SelectStmt opt_with_data
	;

create_as_target :
	qualified_name opt_column_list table_access_method_clause OptWith OnCommitOption OptTableSpace
	;

opt_with_data :
	WITH /*13N*/ DATA_P
	| WITH /*13N*/ NO DATA_P
	| /*empty*/
	;

CreateMatViewStmt :
	CREATE OptNoLog MATERIALIZED VIEW create_mv_target AS SelectStmt opt_with_data
	| CREATE OptNoLog MATERIALIZED VIEW IF_P NOT /*6R*/ EXISTS create_mv_target AS SelectStmt opt_with_data
	;

create_mv_target :
	qualified_name opt_column_list table_access_method_clause opt_reloptions OptTableSpace
	;

OptNoLog :
	UNLOGGED
	| /*empty*/
	;

RefreshMatViewStmt :
	REFRESH MATERIALIZED VIEW opt_concurrently qualified_name opt_with_data
	;

CreateSeqStmt :
	CREATE OptTemp SEQUENCE qualified_name OptSeqOptList
	| CREATE OptTemp SEQUENCE IF_P NOT /*6R*/ EXISTS qualified_name OptSeqOptList
	;

AlterSeqStmt :
	ALTER SEQUENCE qualified_name SeqOptList
	| ALTER SEQUENCE IF_P EXISTS qualified_name SeqOptList
	;

OptSeqOptList :
	SeqOptList
	| /*empty*/
	;

OptParenthesizedSeqOptList :
	'(' /*24L*/ SeqOptList ')' /*24L*/
	| /*empty*/
	;

SeqOptList :
	SeqOptElem
	| SeqOptList SeqOptElem
	;

SeqOptElem :
	AS SimpleTypename
	| CACHE NumericOnly
	| CYCLE
	| NO CYCLE
	| INCREMENT opt_by NumericOnly
	| MAXVALUE NumericOnly
	| MINVALUE NumericOnly
	| NO MAXVALUE
	| NO MINVALUE
	| OWNED BY any_name
	| SEQUENCE NAME_P any_name
	| START opt_with NumericOnly
	| RESTART
	| RESTART opt_with NumericOnly
	;

opt_by :
	BY
	| /*empty*/
	;

NumericOnly :
	FCONST
	| '+' /*17L*/ FCONST
	| '-' /*17L*/ FCONST
	| SignedIconst
	;

NumericOnly_list :
	NumericOnly
	| NumericOnly_list ',' NumericOnly
	;

CreatePLangStmt :
	CREATE opt_or_replace opt_trusted opt_procedural LANGUAGE name
	| CREATE opt_or_replace opt_trusted opt_procedural LANGUAGE name HANDLER handler_name opt_inline_handler opt_validator
	;

opt_trusted :
	TRUSTED
	| /*empty*/
	;

handler_name :
	name
	| name attrs
	;

opt_inline_handler :
	INLINE_P handler_name
	| /*empty*/
	;

validator_clause :
	VALIDATOR handler_name
	| NO VALIDATOR
	;

opt_validator :
	validator_clause
	| /*empty*/
	;

opt_procedural :
	PROCEDURAL
	| /*empty*/
	;

CreateTableSpaceStmt :
	CREATE TABLESPACE name OptTableSpaceOwner LOCATION Sconst opt_reloptions
	;

OptTableSpaceOwner :
	OWNER RoleSpec
	| /*empty*/
	;

DropTableSpaceStmt :
	DROP TABLESPACE name
	| DROP TABLESPACE IF_P EXISTS name
	;

CreateExtensionStmt :
	CREATE EXTENSION name opt_with create_extension_opt_list
	| CREATE EXTENSION IF_P NOT /*6R*/ EXISTS name opt_with create_extension_opt_list
	;

create_extension_opt_list :
	create_extension_opt_list create_extension_opt_item
	| /*empty*/
	;

create_extension_opt_item :
	SCHEMA name
	| VERSION_P NonReservedWord_or_Sconst
	| FROM NonReservedWord_or_Sconst
	| CASCADE
	;

AlterExtensionStmt :
	ALTER EXTENSION name UPDATE alter_extension_opt_list
	;

alter_extension_opt_list :
	alter_extension_opt_list alter_extension_opt_item
	| /*empty*/
	;

alter_extension_opt_item :
	TO NonReservedWord_or_Sconst
	;

AlterExtensionContentsStmt :
	ALTER EXTENSION name add_drop object_type_name name
	| ALTER EXTENSION name add_drop object_type_any_name any_name
	| ALTER EXTENSION name add_drop AGGREGATE aggregate_with_argtypes
	| ALTER EXTENSION name add_drop CAST '(' /*24L*/ Typename AS Typename ')' /*24L*/
	| ALTER EXTENSION name add_drop DOMAIN_P Typename
	| ALTER EXTENSION name add_drop FUNCTION function_with_argtypes
	| ALTER EXTENSION name add_drop OPERATOR /*16L*/ operator_with_argtypes
	| ALTER EXTENSION name add_drop OPERATOR /*16L*/ CLASS any_name USING name
	| ALTER EXTENSION name add_drop OPERATOR /*16L*/ FAMILY any_name USING name
	| ALTER EXTENSION name add_drop PROCEDURE function_with_argtypes
	| ALTER EXTENSION name add_drop ROUTINE function_with_argtypes
	| ALTER EXTENSION name add_drop TRANSFORM FOR Typename LANGUAGE name
	| ALTER EXTENSION name add_drop TYPE_P Typename
	;

CreateFdwStmt :
	CREATE FOREIGN DATA_P WRAPPER name opt_fdw_options create_generic_options
	;

fdw_option :
	HANDLER handler_name
	| NO HANDLER
	| VALIDATOR handler_name
	| NO VALIDATOR
	;

fdw_options :
	fdw_option
	| fdw_options fdw_option
	;

opt_fdw_options :
	fdw_options
	| /*empty*/
	;

AlterFdwStmt :
	ALTER FOREIGN DATA_P WRAPPER name opt_fdw_options alter_generic_options
	| ALTER FOREIGN DATA_P WRAPPER name fdw_options
	;

create_generic_options :
	OPTIONS '(' /*24L*/ generic_option_list ')' /*24L*/
	| /*empty*/
	;

generic_option_list :
	generic_option_elem
	| generic_option_list ',' generic_option_elem
	;

alter_generic_options :
	OPTIONS '(' /*24L*/ alter_generic_option_list ')' /*24L*/
	;

alter_generic_option_list :
	alter_generic_option_elem
	| alter_generic_option_list ',' alter_generic_option_elem
	;

alter_generic_option_elem :
	generic_option_elem
	| SET /*1N*/ generic_option_elem
	| ADD_P generic_option_elem
	| DROP generic_option_name
	;

generic_option_elem :
	generic_option_name generic_option_arg
	;

generic_option_name :
	ColLabel
	;

generic_option_arg :
	Sconst
	;

CreateForeignServerStmt :
	CREATE SERVER name opt_type opt_foreign_server_version FOREIGN DATA_P WRAPPER name create_generic_options
	| CREATE SERVER IF_P NOT /*6R*/ EXISTS name opt_type opt_foreign_server_version FOREIGN DATA_P WRAPPER name create_generic_options
	;

opt_type :
	TYPE_P Sconst
	| /*empty*/
	;

foreign_server_version :
	VERSION_P Sconst
	| VERSION_P NULL_P
	;

opt_foreign_server_version :
	foreign_server_version
	| /*empty*/
	;

AlterForeignServerStmt :
	ALTER SERVER name foreign_server_version alter_generic_options
	| ALTER SERVER name foreign_server_version
	| ALTER SERVER name alter_generic_options
	;

CreateForeignTableStmt :
	CREATE FOREIGN TABLE qualified_name '(' /*24L*/ OptTableElementList ')' /*24L*/ OptInherit SERVER name create_generic_options
	| CREATE FOREIGN TABLE IF_P NOT /*6R*/ EXISTS qualified_name '(' /*24L*/ OptTableElementList ')' /*24L*/ OptInherit SERVER name create_generic_options
	| CREATE FOREIGN TABLE qualified_name PARTITION /*15N*/ OF qualified_name OptTypedTableElementList PartitionBoundSpec SERVER name create_generic_options
	| CREATE FOREIGN TABLE IF_P NOT /*6R*/ EXISTS qualified_name PARTITION /*15N*/ OF qualified_name OptTypedTableElementList PartitionBoundSpec SERVER name create_generic_options
	;

ImportForeignSchemaStmt :
	IMPORT_P FOREIGN SCHEMA name import_qualification FROM SERVER name INTO name create_generic_options
	;

import_qualification_type :
	LIMIT TO
	| EXCEPT /*2L*/
	;

import_qualification :
	import_qualification_type '(' /*24L*/ relation_expr_list ')' /*24L*/
	| /*empty*/
	;

CreateUserMappingStmt :
	CREATE USER MAPPING FOR auth_ident SERVER name create_generic_options
	| CREATE USER MAPPING IF_P NOT /*6R*/ EXISTS FOR auth_ident SERVER name create_generic_options
	;

auth_ident :
	RoleSpec
	| USER
	;

DropUserMappingStmt :
	DROP USER MAPPING FOR auth_ident SERVER name
	| DROP USER MAPPING IF_P EXISTS FOR auth_ident SERVER name
	;

AlterUserMappingStmt :
	ALTER USER MAPPING FOR auth_ident SERVER name alter_generic_options
	;

CreatePolicyStmt :
	CREATE POLICY name ON qualified_name RowSecurityDefaultPermissive RowSecurityDefaultForCmd RowSecurityDefaultToRole RowSecurityOptionalExpr RowSecurityOptionalWithCheck
	;

AlterPolicyStmt :
	ALTER POLICY name ON qualified_name RowSecurityOptionalToRole RowSecurityOptionalExpr RowSecurityOptionalWithCheck
	;

RowSecurityOptionalExpr :
	USING '(' /*24L*/ a_expr ')' /*24L*/
	| /*empty*/
	;

RowSecurityOptionalWithCheck :
	WITH /*13N*/ CHECK '(' /*24L*/ a_expr ')' /*24L*/
	| /*empty*/
	;

RowSecurityDefaultToRole :
	TO role_list
	| /*empty*/
	;

RowSecurityOptionalToRole :
	TO role_list
	| /*empty*/
	;

RowSecurityDefaultPermissive :
	AS IDENT /*15N*/
	| /*empty*/
	;

RowSecurityDefaultForCmd :
	FOR row_security_cmd
	| /*empty*/
	;

row_security_cmd :
	ALL
	| SELECT
	| INSERT
	| UPDATE
	| DELETE_P
	;

CreateAmStmt :
	CREATE ACCESS METHOD name TYPE_P am_type HANDLER handler_name
	;

am_type :
	INDEX
	| TABLE
	;

CreateTrigStmt :
	CREATE opt_or_replace TRIGGER name TriggerActionTime TriggerEvents ON qualified_name TriggerReferencing TriggerForSpec TriggerWhen EXECUTE FUNCTION_or_PROCEDURE func_name '(' /*24L*/ TriggerFuncArgs ')' /*24L*/
	| CREATE opt_or_replace CONSTRAINT TRIGGER name AFTER TriggerEvents ON qualified_name OptConstrFromTable ConstraintAttributeSpec FOR EACH ROW TriggerWhen EXECUTE FUNCTION_or_PROCEDURE func_name '(' /*24L*/ TriggerFuncArgs ')' /*24L*/
	;

TriggerActionTime :
	BEFORE
	| AFTER
	| INSTEAD OF
	;

TriggerEvents :
	TriggerOneEvent
	| TriggerEvents OR /*4L*/ TriggerOneEvent
	;

TriggerOneEvent :
	INSERT
	| DELETE_P
	| UPDATE
	| UPDATE OF columnList
	| TRUNCATE
	;

TriggerReferencing :
	REFERENCING TriggerTransitions
	| /*empty*/
	;

TriggerTransitions :
	TriggerTransition
	| TriggerTransitions TriggerTransition
	;

TriggerTransition :
	TransitionOldOrNew TransitionRowOrTable opt_as TransitionRelName
	;

TransitionOldOrNew :
	NEW
	| OLD
	;

TransitionRowOrTable :
	TABLE
	| ROW
	;

TransitionRelName :
	ColId
	;

TriggerForSpec :
	FOR TriggerForOptEach TriggerForType
	| /*empty*/
	;

TriggerForOptEach :
	EACH
	| /*empty*/
	;

TriggerForType :
	ROW
	| STATEMENT
	;

TriggerWhen :
	WHEN '(' /*24L*/ a_expr ')' /*24L*/
	| /*empty*/
	;

FUNCTION_or_PROCEDURE :
	FUNCTION
	| PROCEDURE
	;

TriggerFuncArgs :
	TriggerFuncArg
	| TriggerFuncArgs ',' TriggerFuncArg
	| /*empty*/
	;

TriggerFuncArg :
	Iconst
	| FCONST
	| Sconst
	| ColLabel
	;

OptConstrFromTable :
	FROM qualified_name
	| /*empty*/
	;

ConstraintAttributeSpec :
	/*empty*/
	| ConstraintAttributeSpec ConstraintAttributeElem
	;

ConstraintAttributeElem :
	NOT /*6R*/ DEFERRABLE
	| DEFERRABLE
	| INITIALLY IMMEDIATE
	| INITIALLY DEFERRED
	| NOT /*6R*/ VALID
	| NO INHERIT
	;

CreateEventTrigStmt :
	CREATE EVENT TRIGGER name ON ColLabel EXECUTE FUNCTION_or_PROCEDURE func_name '(' /*24L*/ ')' /*24L*/
	| CREATE EVENT TRIGGER name ON ColLabel WHEN event_trigger_when_list EXECUTE FUNCTION_or_PROCEDURE func_name '(' /*24L*/ ')' /*24L*/
	;

event_trigger_when_list :
	event_trigger_when_item
	| event_trigger_when_list AND /*5L*/ event_trigger_when_item
	;

event_trigger_when_item :
	ColId IN_P /*9N*/ '(' /*24L*/ event_trigger_value_list ')' /*24L*/
	;

event_trigger_value_list :
	SCONST
	| event_trigger_value_list ',' SCONST
	;

AlterEventTrigStmt :
	ALTER EVENT TRIGGER name enable_trigger
	;

enable_trigger :
	ENABLE_P
	| ENABLE_P REPLICA
	| ENABLE_P ALWAYS
	| DISABLE_P
	;

CreateAssertionStmt :
	CREATE ASSERTION any_name CHECK '(' /*24L*/ a_expr ')' /*24L*/ ConstraintAttributeSpec
	;

DefineStmt :
	CREATE opt_or_replace AGGREGATE func_name aggr_args definition
	| CREATE opt_or_replace AGGREGATE func_name old_aggr_definition
	| CREATE OPERATOR /*16L*/ any_operator definition
	| CREATE TYPE_P any_name definition
	| CREATE TYPE_P any_name
	| CREATE TYPE_P any_name AS '(' /*24L*/ OptTableFuncElementList ')' /*24L*/
	| CREATE TYPE_P any_name AS ENUM_P '(' /*24L*/ opt_enum_val_list ')' /*24L*/
	| CREATE TYPE_P any_name AS RANGE /*15N*/ definition
	| CREATE TEXT_P SEARCH PARSER any_name definition
	| CREATE TEXT_P SEARCH DICTIONARY any_name definition
	| CREATE TEXT_P SEARCH TEMPLATE any_name definition
	| CREATE TEXT_P SEARCH CONFIGURATION any_name definition
	| CREATE COLLATION any_name definition
	| CREATE COLLATION IF_P NOT /*6R*/ EXISTS any_name definition
	| CREATE COLLATION any_name FROM any_name
	| CREATE COLLATION IF_P NOT /*6R*/ EXISTS any_name FROM any_name
	;

definition :
	'(' /*24L*/ def_list ')' /*24L*/
	;

def_list :
	def_elem
	| def_list ',' def_elem
	;

def_elem :
	ColLabel '=' /*8N*/ def_arg
	| ColLabel
	;

def_arg :
	func_type
	| reserved_keyword
	| qual_all_Op
	| NumericOnly
	| Sconst
	| NONE
	;

old_aggr_definition :
	'(' /*24L*/ old_aggr_list ')' /*24L*/
	;

old_aggr_list :
	old_aggr_elem
	| old_aggr_list ',' old_aggr_elem
	;

old_aggr_elem :
	IDENT /*15N*/ '=' /*8N*/ def_arg
	;

opt_enum_val_list :
	enum_val_list
	| /*empty*/
	;

enum_val_list :
	Sconst
	| enum_val_list ',' Sconst
	;

AlterEnumStmt :
	ALTER TYPE_P any_name ADD_P VALUE_P /*12N*/ opt_if_not_exists Sconst
	| ALTER TYPE_P any_name ADD_P VALUE_P /*12N*/ opt_if_not_exists Sconst BEFORE Sconst
	| ALTER TYPE_P any_name ADD_P VALUE_P /*12N*/ opt_if_not_exists Sconst AFTER Sconst
	| ALTER TYPE_P any_name RENAME VALUE_P /*12N*/ Sconst TO Sconst
	;

opt_if_not_exists :
	IF_P NOT /*6R*/ EXISTS
	| /*empty*/
	;

CreateOpClassStmt :
	CREATE OPERATOR /*16L*/ CLASS any_name opt_default FOR TYPE_P Typename USING name opt_opfamily AS opclass_item_list
	;

opclass_item_list :
	opclass_item
	| opclass_item_list ',' opclass_item
	;

opclass_item :
	OPERATOR /*16L*/ Iconst any_operator opclass_purpose opt_recheck
	| OPERATOR /*16L*/ Iconst operator_with_argtypes opclass_purpose opt_recheck
	| FUNCTION Iconst function_with_argtypes
	| FUNCTION Iconst '(' /*24L*/ type_list ')' /*24L*/ function_with_argtypes
	| STORAGE Typename
	;

opt_default :
	DEFAULT
	| /*empty*/
	;

opt_opfamily :
	FAMILY any_name
	| /*empty*/
	;

opclass_purpose :
	FOR SEARCH
	| FOR ORDER BY any_name
	| /*empty*/
	;

opt_recheck :
	RECHECK
	| /*empty*/
	;

CreateOpFamilyStmt :
	CREATE OPERATOR /*16L*/ FAMILY any_name USING name
	;

AlterOpFamilyStmt :
	ALTER OPERATOR /*16L*/ FAMILY any_name USING name ADD_P opclass_item_list
	| ALTER OPERATOR /*16L*/ FAMILY any_name USING name DROP opclass_drop_list
	;

opclass_drop_list :
	opclass_drop
	| opclass_drop_list ',' opclass_drop
	;

opclass_drop :
	OPERATOR /*16L*/ Iconst '(' /*24L*/ type_list ')' /*24L*/
	| FUNCTION Iconst '(' /*24L*/ type_list ')' /*24L*/
	;

DropOpClassStmt :
	DROP OPERATOR /*16L*/ CLASS any_name USING name opt_drop_behavior
	| DROP OPERATOR /*16L*/ CLASS IF_P EXISTS any_name USING name opt_drop_behavior
	;

DropOpFamilyStmt :
	DROP OPERATOR /*16L*/ FAMILY any_name USING name opt_drop_behavior
	| DROP OPERATOR /*16L*/ FAMILY IF_P EXISTS any_name USING name opt_drop_behavior
	;

DropOwnedStmt :
	DROP OWNED BY role_list opt_drop_behavior
	;

ReassignOwnedStmt :
	REASSIGN OWNED BY role_list TO RoleSpec
	;

DropStmt :
	DROP object_type_any_name IF_P EXISTS any_name_list opt_drop_behavior
	| DROP object_type_any_name any_name_list opt_drop_behavior
	| DROP drop_type_name IF_P EXISTS name_list opt_drop_behavior
	| DROP drop_type_name name_list opt_drop_behavior
	| DROP object_type_name_on_any_name name ON any_name opt_drop_behavior
	| DROP object_type_name_on_any_name IF_P EXISTS name ON any_name opt_drop_behavior
	| DROP TYPE_P type_name_list opt_drop_behavior
	| DROP TYPE_P IF_P EXISTS type_name_list opt_drop_behavior
	| DROP DOMAIN_P type_name_list opt_drop_behavior
	| DROP DOMAIN_P IF_P EXISTS type_name_list opt_drop_behavior
	| DROP INDEX CONCURRENTLY any_name_list opt_drop_behavior
	| DROP INDEX CONCURRENTLY IF_P EXISTS any_name_list opt_drop_behavior
	;

object_type_any_name :
	TABLE
	| SEQUENCE
	| VIEW
	| MATERIALIZED VIEW
	| INDEX
	| FOREIGN TABLE
	| COLLATION
	| CONVERSION_P
	| STATISTICS
	| TEXT_P SEARCH PARSER
	| TEXT_P SEARCH DICTIONARY
	| TEXT_P SEARCH TEMPLATE
	| TEXT_P SEARCH CONFIGURATION
	;

object_type_name :
	drop_type_name
	| DATABASE
	| ROLE
	| SUBSCRIPTION
	| TABLESPACE
	;

drop_type_name :
	ACCESS METHOD
	| EVENT TRIGGER
	| EXTENSION
	| FOREIGN DATA_P WRAPPER
	| opt_procedural LANGUAGE
	| PUBLICATION
	| SCHEMA
	| SERVER
	;

object_type_name_on_any_name :
	POLICY
	| RULE
	| TRIGGER
	;

any_name_list :
	any_name
	| any_name_list ',' any_name
	;

any_name :
	ColId
	| ColId attrs
	;

attrs :
	'.' /*26L*/ attr_name
	| attrs '.' /*26L*/ attr_name
	;

type_name_list :
	Typename
	| type_name_list ',' Typename
	;

TruncateStmt :
	TRUNCATE opt_table relation_expr_list opt_restart_seqs opt_drop_behavior
	;

opt_restart_seqs :
	CONTINUE_P IDENTITY_P
	| RESTART IDENTITY_P
	| /*empty*/
	;

CommentStmt :
	COMMENT ON object_type_any_name any_name IS /*7N*/ comment_text
	| COMMENT ON COLUMN any_name IS /*7N*/ comment_text
	| COMMENT ON object_type_name name IS /*7N*/ comment_text
	| COMMENT ON TYPE_P Typename IS /*7N*/ comment_text
	| COMMENT ON DOMAIN_P Typename IS /*7N*/ comment_text
	| COMMENT ON AGGREGATE aggregate_with_argtypes IS /*7N*/ comment_text
	| COMMENT ON FUNCTION function_with_argtypes IS /*7N*/ comment_text
	| COMMENT ON OPERATOR /*16L*/ operator_with_argtypes IS /*7N*/ comment_text
	| COMMENT ON CONSTRAINT name ON any_name IS /*7N*/ comment_text
	| COMMENT ON CONSTRAINT name ON DOMAIN_P any_name IS /*7N*/ comment_text
	| COMMENT ON object_type_name_on_any_name name ON any_name IS /*7N*/ comment_text
	| COMMENT ON PROCEDURE function_with_argtypes IS /*7N*/ comment_text
	| COMMENT ON ROUTINE function_with_argtypes IS /*7N*/ comment_text
	| COMMENT ON TRANSFORM FOR Typename LANGUAGE name IS /*7N*/ comment_text
	| COMMENT ON OPERATOR /*16L*/ CLASS any_name USING name IS /*7N*/ comment_text
	| COMMENT ON OPERATOR /*16L*/ FAMILY any_name USING name IS /*7N*/ comment_text
	| COMMENT ON LARGE_P OBJECT_P /*12N*/ NumericOnly IS /*7N*/ comment_text
	| COMMENT ON CAST '(' /*24L*/ Typename AS Typename ')' /*24L*/ IS /*7N*/ comment_text
	;

comment_text :
	Sconst
	| NULL_P
	;

SecLabelStmt :
	SECURITY LABEL opt_provider ON object_type_any_name any_name IS /*7N*/ security_label
	| SECURITY LABEL opt_provider ON COLUMN any_name IS /*7N*/ security_label
	| SECURITY LABEL opt_provider ON object_type_name name IS /*7N*/ security_label
	| SECURITY LABEL opt_provider ON TYPE_P Typename IS /*7N*/ security_label
	| SECURITY LABEL opt_provider ON DOMAIN_P Typename IS /*7N*/ security_label
	| SECURITY LABEL opt_provider ON AGGREGATE aggregate_with_argtypes IS /*7N*/ security_label
	| SECURITY LABEL opt_provider ON FUNCTION function_with_argtypes IS /*7N*/ security_label
	| SECURITY LABEL opt_provider ON LARGE_P OBJECT_P /*12N*/ NumericOnly IS /*7N*/ security_label
	| SECURITY LABEL opt_provider ON PROCEDURE function_with_argtypes IS /*7N*/ security_label
	| SECURITY LABEL opt_provider ON ROUTINE function_with_argtypes IS /*7N*/ security_label
	;

opt_provider :
	FOR NonReservedWord_or_Sconst
	| /*empty*/
	;

security_label :
	Sconst
	| NULL_P
	;

FetchStmt :
	FETCH fetch_args
	| MOVE fetch_args
	;

fetch_args :
	cursor_name
	| from_in cursor_name
	| NEXT opt_from_in cursor_name
	| PRIOR opt_from_in cursor_name
	| FIRST_P opt_from_in cursor_name
	| LAST_P opt_from_in cursor_name
	| ABSOLUTE_P SignedIconst opt_from_in cursor_name
	| RELATIVE_P SignedIconst opt_from_in cursor_name
	| SignedIconst opt_from_in cursor_name
	| ALL opt_from_in cursor_name
	| FORWARD opt_from_in cursor_name
	| FORWARD SignedIconst opt_from_in cursor_name
	| FORWARD ALL opt_from_in cursor_name
	| BACKWARD opt_from_in cursor_name
	| BACKWARD SignedIconst opt_from_in cursor_name
	| BACKWARD ALL opt_from_in cursor_name
	;

from_in :
	FROM
	| IN_P /*9N*/
	;

opt_from_in :
	from_in
	| /*empty*/
	;

GrantStmt :
	GRANT privileges ON privilege_target TO grantee_list opt_grant_grant_option opt_granted_by
	;

RevokeStmt :
	REVOKE privileges ON privilege_target FROM grantee_list opt_granted_by opt_drop_behavior
	| REVOKE GRANT OPTION FOR privileges ON privilege_target FROM grantee_list opt_granted_by opt_drop_behavior
	;

privileges :
	privilege_list
	| ALL
	| ALL PRIVILEGES
	| ALL '(' /*24L*/ columnList ')' /*24L*/
	| ALL PRIVILEGES '(' /*24L*/ columnList ')' /*24L*/
	;

privilege_list :
	privilege
	| privilege_list ',' privilege
	;

privilege :
	SELECT opt_column_list
	| REFERENCES opt_column_list
	| CREATE opt_column_list
	| ALTER SYSTEM_P
	| ColId opt_column_list
	;

parameter_name_list :
	parameter_name
	| parameter_name_list ',' parameter_name
	;

parameter_name :
	ColId
	| parameter_name '.' /*26L*/ ColId
	;

privilege_target :
	qualified_name_list
	| TABLE qualified_name_list
	| SEQUENCE qualified_name_list
	| FOREIGN DATA_P WRAPPER name_list
	| FOREIGN SERVER name_list
	| FUNCTION function_with_argtypes_list
	| PROCEDURE function_with_argtypes_list
	| ROUTINE function_with_argtypes_list
	| DATABASE name_list
	| DOMAIN_P any_name_list
	| LANGUAGE name_list
	| LARGE_P OBJECT_P /*12N*/ NumericOnly_list
	| PARAMETER parameter_name_list
	| SCHEMA name_list
	| TABLESPACE name_list
	| TYPE_P any_name_list
	| ALL TABLES IN_P /*9N*/ SCHEMA name_list
	| ALL SEQUENCES IN_P /*9N*/ SCHEMA name_list
	| ALL FUNCTIONS IN_P /*9N*/ SCHEMA name_list
	| ALL PROCEDURES IN_P /*9N*/ SCHEMA name_list
	| ALL ROUTINES IN_P /*9N*/ SCHEMA name_list
	;

grantee_list :
	grantee
	| grantee_list ',' grantee
	;

grantee :
	RoleSpec
	| GROUP_P RoleSpec
	;

opt_grant_grant_option :
	WITH /*13N*/ GRANT OPTION
	| /*empty*/
	;

GrantRoleStmt :
	GRANT privilege_list TO role_list opt_granted_by
	| GRANT privilege_list TO role_list WITH /*13N*/ grant_role_opt_list opt_granted_by
	;

RevokeRoleStmt :
	REVOKE privilege_list FROM role_list opt_granted_by opt_drop_behavior
	| REVOKE ColId OPTION FOR privilege_list FROM role_list opt_granted_by opt_drop_behavior
	;

grant_role_opt_list :
	grant_role_opt_list ',' grant_role_opt
	| grant_role_opt
	;

grant_role_opt :
	ColLabel grant_role_opt_value
	;

grant_role_opt_value :
	OPTION
	| TRUE_P
	| FALSE_P
	;

opt_granted_by :
	GRANTED BY RoleSpec
	| /*empty*/
	;

AlterDefaultPrivilegesStmt :
	ALTER DEFAULT PRIVILEGES DefACLOptionList DefACLAction
	;

DefACLOptionList :
	DefACLOptionList DefACLOption
	| /*empty*/
	;

DefACLOption :
	IN_P /*9N*/ SCHEMA name_list
	| FOR ROLE role_list
	| FOR USER role_list
	;

DefACLAction :
	GRANT privileges ON defacl_privilege_target TO grantee_list opt_grant_grant_option
	| REVOKE privileges ON defacl_privilege_target FROM grantee_list opt_drop_behavior
	| REVOKE GRANT OPTION FOR privileges ON defacl_privilege_target FROM grantee_list opt_drop_behavior
	;

defacl_privilege_target :
	TABLES
	| FUNCTIONS
	| ROUTINES
	| SEQUENCES
	| TYPES_P
	| SCHEMAS
	;

IndexStmt :
	CREATE opt_unique INDEX opt_concurrently opt_single_name ON relation_expr access_method_clause '(' /*24L*/ index_params ')' /*24L*/ opt_include opt_unique_null_treatment opt_reloptions OptTableSpace where_clause
	| CREATE opt_unique INDEX opt_concurrently IF_P NOT /*6R*/ EXISTS name ON relation_expr access_method_clause '(' /*24L*/ index_params ')' /*24L*/ opt_include opt_unique_null_treatment opt_reloptions OptTableSpace where_clause
	;

opt_unique :
	UNIQUE /*11N*/
	| /*empty*/
	;

access_method_clause :
	USING name
	| /*empty*/
	;

index_params :
	index_elem
	| index_params ',' index_elem
	;

index_elem_options :
	opt_collate opt_qualified_name opt_asc_desc opt_nulls_order
	| opt_collate any_name reloptions opt_asc_desc opt_nulls_order
	;

index_elem :
	ColId index_elem_options
	| func_expr_windowless index_elem_options
	| '(' /*24L*/ a_expr ')' /*24L*/ index_elem_options
	;

opt_include :
	INCLUDE '(' /*24L*/ index_including_params ')' /*24L*/
	| /*empty*/
	;

index_including_params :
	index_elem
	| index_including_params ',' index_elem
	;

opt_collate :
	COLLATE /*21L*/ any_name
	| /*empty*/
	;

opt_asc_desc :
	ASC
	| DESC
	| /*empty*/
	;

opt_nulls_order :
	NULLS_LA FIRST_P
	| NULLS_LA LAST_P
	| /*empty*/
	;

CreateFunctionStmt :
	CREATE opt_or_replace FUNCTION func_name func_args_with_defaults RETURNS func_return opt_createfunc_opt_list opt_routine_body
	| CREATE opt_or_replace FUNCTION func_name func_args_with_defaults RETURNS TABLE '(' /*24L*/ table_func_column_list ')' /*24L*/ opt_createfunc_opt_list opt_routine_body
	| CREATE opt_or_replace FUNCTION func_name func_args_with_defaults opt_createfunc_opt_list opt_routine_body
	| CREATE opt_or_replace PROCEDURE func_name func_args_with_defaults opt_createfunc_opt_list opt_routine_body
	;

opt_or_replace :
	OR /*4L*/ REPLACE
	| /*empty*/
	;

func_args :
	'(' /*24L*/ func_args_list ')' /*24L*/
	| '(' /*24L*/ ')' /*24L*/
	;

func_args_list :
	func_arg
	| func_args_list ',' func_arg
	;

function_with_argtypes_list :
	function_with_argtypes
	| function_with_argtypes_list ',' function_with_argtypes
	;

function_with_argtypes :
	func_name func_args
	| type_func_name_keyword
	| ColId
	| ColId indirection
	;

func_args_with_defaults :
	'(' /*24L*/ func_args_with_defaults_list ')' /*24L*/
	| '(' /*24L*/ ')' /*24L*/
	;

func_args_with_defaults_list :
	func_arg_with_default
	| func_args_with_defaults_list ',' func_arg_with_default
	;

func_arg :
	arg_class param_name func_type
	| param_name arg_class func_type
	| param_name func_type
	| arg_class func_type
	| func_type
	;

arg_class :
	IN_P /*9N*/
	| OUT_P
	| INOUT
	| IN_P /*9N*/ OUT_P
	| VARIADIC
	;

param_name :
	type_function_name
	;

func_return :
	func_type
	;

func_type :
	Typename
	| type_function_name attrs '%' /*18L*/ TYPE_P
	| SETOF type_function_name attrs '%' /*18L*/ TYPE_P
	;

func_arg_with_default :
	func_arg
	| func_arg DEFAULT a_expr
	| func_arg '=' /*8N*/ a_expr
	;

aggr_arg :
	func_arg
	;

aggr_args :
	'(' /*24L*/ '*' /*18L*/ ')' /*24L*/
	| '(' /*24L*/ aggr_args_list ')' /*24L*/
	| '(' /*24L*/ ORDER BY aggr_args_list ')' /*24L*/
	| '(' /*24L*/ aggr_args_list ORDER BY aggr_args_list ')' /*24L*/
	;

aggr_args_list :
	aggr_arg
	| aggr_args_list ',' aggr_arg
	;

aggregate_with_argtypes :
	func_name aggr_args
	;

aggregate_with_argtypes_list :
	aggregate_with_argtypes
	| aggregate_with_argtypes_list ',' aggregate_with_argtypes
	;

opt_createfunc_opt_list :
	createfunc_opt_list
	| /*empty*/
	;

createfunc_opt_list :
	createfunc_opt_item
	| createfunc_opt_list createfunc_opt_item
	;

common_func_opt_item :
	CALLED ON NULL_P INPUT_P
	| RETURNS NULL_P ON NULL_P INPUT_P
	| STRICT_P
	| IMMUTABLE
	| STABLE
	| VOLATILE
	| EXTERNAL SECURITY DEFINER
	| EXTERNAL SECURITY INVOKER
	| SECURITY DEFINER
	| SECURITY INVOKER
	| LEAKPROOF
	| NOT /*6R*/ LEAKPROOF
	| COST NumericOnly
	| ROWS /*15N*/ NumericOnly
	| SUPPORT any_name
	| FunctionSetResetClause
	| PARALLEL ColId
	;

createfunc_opt_item :
	AS func_as
	| LANGUAGE NonReservedWord_or_Sconst
	| TRANSFORM transform_type_list
	| WINDOW
	| common_func_opt_item
	;

func_as :
	Sconst
	| Sconst ',' Sconst
	;

ReturnStmt :
	RETURN a_expr
	;

opt_routine_body :
	ReturnStmt
	| BEGIN_P ATOMIC routine_body_stmt_list END_P
	| /*empty*/
	;

routine_body_stmt_list :
	routine_body_stmt_list routine_body_stmt ';'
	| /*empty*/
	;

routine_body_stmt :
	stmt
	| ReturnStmt
	;

transform_type_list :
	FOR TYPE_P Typename
	| transform_type_list ',' FOR TYPE_P Typename
	;

opt_definition :
	WITH /*13N*/ definition
	| /*empty*/
	;

table_func_column :
	param_name func_type
	;

table_func_column_list :
	table_func_column
	| table_func_column_list ',' table_func_column
	;

AlterFunctionStmt :
	ALTER FUNCTION function_with_argtypes alterfunc_opt_list opt_restrict
	| ALTER PROCEDURE function_with_argtypes alterfunc_opt_list opt_restrict
	| ALTER ROUTINE function_with_argtypes alterfunc_opt_list opt_restrict
	;

alterfunc_opt_list :
	common_func_opt_item
	| alterfunc_opt_list common_func_opt_item
	;

opt_restrict :
	RESTRICT
	| /*empty*/
	;

RemoveFuncStmt :
	DROP FUNCTION function_with_argtypes_list opt_drop_behavior
	| DROP FUNCTION IF_P EXISTS function_with_argtypes_list opt_drop_behavior
	| DROP PROCEDURE function_with_argtypes_list opt_drop_behavior
	| DROP PROCEDURE IF_P EXISTS function_with_argtypes_list opt_drop_behavior
	| DROP ROUTINE function_with_argtypes_list opt_drop_behavior
	| DROP ROUTINE IF_P EXISTS function_with_argtypes_list opt_drop_behavior
	;

RemoveAggrStmt :
	DROP AGGREGATE aggregate_with_argtypes_list opt_drop_behavior
	| DROP AGGREGATE IF_P EXISTS aggregate_with_argtypes_list opt_drop_behavior
	;

RemoveOperStmt :
	DROP OPERATOR /*16L*/ operator_with_argtypes_list opt_drop_behavior
	| DROP OPERATOR /*16L*/ IF_P EXISTS operator_with_argtypes_list opt_drop_behavior
	;

oper_argtypes :
	'(' /*24L*/ Typename ')' /*24L*/
	| '(' /*24L*/ Typename ',' Typename ')' /*24L*/
	| '(' /*24L*/ NONE ',' Typename ')' /*24L*/
	| '(' /*24L*/ Typename ',' NONE ')' /*24L*/
	;

any_operator :
	all_Op
	| ColId '.' /*26L*/ any_operator
	;

operator_with_argtypes_list :
	operator_with_argtypes
	| operator_with_argtypes_list ',' operator_with_argtypes
	;

operator_with_argtypes :
	any_operator oper_argtypes
	;

DoStmt :
	DO dostmt_opt_list
	;

dostmt_opt_list :
	dostmt_opt_item
	| dostmt_opt_list dostmt_opt_item
	;

dostmt_opt_item :
	Sconst
	| LANGUAGE NonReservedWord_or_Sconst
	;

CreateCastStmt :
	CREATE CAST '(' /*24L*/ Typename AS Typename ')' /*24L*/ WITH /*13N*/ FUNCTION function_with_argtypes cast_context
	| CREATE CAST '(' /*24L*/ Typename AS Typename ')' /*24L*/ WITHOUT /*13N*/ FUNCTION cast_context
	| CREATE CAST '(' /*24L*/ Typename AS Typename ')' /*24L*/ WITH /*13N*/ INOUT cast_context
	;

cast_context :
	AS IMPLICIT_P
	| AS ASSIGNMENT
	| /*empty*/
	;

DropCastStmt :
	DROP CAST opt_if_exists '(' /*24L*/ Typename AS Typename ')' /*24L*/ opt_drop_behavior
	;

opt_if_exists :
	IF_P EXISTS
	| /*empty*/
	;

CreateTransformStmt :
	CREATE opt_or_replace TRANSFORM FOR Typename LANGUAGE name '(' /*24L*/ transform_element_list ')' /*24L*/
	;

transform_element_list :
	FROM SQL_P WITH /*13N*/ FUNCTION function_with_argtypes ',' TO SQL_P WITH /*13N*/ FUNCTION function_with_argtypes
	| TO SQL_P WITH /*13N*/ FUNCTION function_with_argtypes ',' FROM SQL_P WITH /*13N*/ FUNCTION function_with_argtypes
	| FROM SQL_P WITH /*13N*/ FUNCTION function_with_argtypes
	| TO SQL_P WITH /*13N*/ FUNCTION function_with_argtypes
	;

DropTransformStmt :
	DROP TRANSFORM opt_if_exists FOR Typename LANGUAGE name opt_drop_behavior
	;

ReindexStmt :
	REINDEX opt_reindex_option_list reindex_target_relation opt_concurrently qualified_name
	| REINDEX opt_reindex_option_list SCHEMA opt_concurrently name
	| REINDEX opt_reindex_option_list reindex_target_all opt_concurrently opt_single_name
	;

reindex_target_relation :
	INDEX
	| TABLE
	;

reindex_target_all :
	SYSTEM_P
	| DATABASE
	;

opt_reindex_option_list :
	'(' /*24L*/ utility_option_list ')' /*24L*/
	| /*empty*/
	;

AlterTblSpcStmt :
	ALTER TABLESPACE name SET /*1N*/ reloptions
	| ALTER TABLESPACE name RESET reloptions
	;

RenameStmt :
	ALTER AGGREGATE aggregate_with_argtypes RENAME TO name
	| ALTER COLLATION any_name RENAME TO name
	| ALTER CONVERSION_P any_name RENAME TO name
	| ALTER DATABASE name RENAME TO name
	| ALTER DOMAIN_P any_name RENAME TO name
	| ALTER DOMAIN_P any_name RENAME CONSTRAINT name TO name
	| ALTER FOREIGN DATA_P WRAPPER name RENAME TO name
	| ALTER FUNCTION function_with_argtypes RENAME TO name
	| ALTER GROUP_P RoleId RENAME TO RoleId
	| ALTER opt_procedural LANGUAGE name RENAME TO name
	| ALTER OPERATOR /*16L*/ CLASS any_name USING name RENAME TO name
	| ALTER OPERATOR /*16L*/ FAMILY any_name USING name RENAME TO name
	| ALTER POLICY name ON qualified_name RENAME TO name
	| ALTER POLICY IF_P EXISTS name ON qualified_name RENAME TO name
	| ALTER PROCEDURE function_with_argtypes RENAME TO name
	| ALTER PUBLICATION name RENAME TO name
	| ALTER ROUTINE function_with_argtypes RENAME TO name
	| ALTER SCHEMA name RENAME TO name
	| ALTER SERVER name RENAME TO name
	| ALTER SUBSCRIPTION name RENAME TO name
	| ALTER TABLE relation_expr RENAME TO name
	| ALTER TABLE IF_P EXISTS relation_expr RENAME TO name
	| ALTER SEQUENCE qualified_name RENAME TO name
	| ALTER SEQUENCE IF_P EXISTS qualified_name RENAME TO name
	| ALTER VIEW qualified_name RENAME TO name
	| ALTER VIEW IF_P EXISTS qualified_name RENAME TO name
	| ALTER MATERIALIZED VIEW qualified_name RENAME TO name
	| ALTER MATERIALIZED VIEW IF_P EXISTS qualified_name RENAME TO name
	| ALTER INDEX qualified_name RENAME TO name
	| ALTER INDEX IF_P EXISTS qualified_name RENAME TO name
	| ALTER FOREIGN TABLE relation_expr RENAME TO name
	| ALTER FOREIGN TABLE IF_P EXISTS relation_expr RENAME TO name
	| ALTER TABLE relation_expr RENAME opt_column name TO name
	| ALTER TABLE IF_P EXISTS relation_expr RENAME opt_column name TO name
	| ALTER VIEW qualified_name RENAME opt_column name TO name
	| ALTER VIEW IF_P EXISTS qualified_name RENAME opt_column name TO name
	| ALTER MATERIALIZED VIEW qualified_name RENAME opt_column name TO name
	| ALTER MATERIALIZED VIEW IF_P EXISTS qualified_name RENAME opt_column name TO name
	| ALTER TABLE relation_expr RENAME CONSTRAINT name TO name
	| ALTER TABLE IF_P EXISTS relation_expr RENAME CONSTRAINT name TO name
	| ALTER FOREIGN TABLE relation_expr RENAME opt_column name TO name
	| ALTER FOREIGN TABLE IF_P EXISTS relation_expr RENAME opt_column name TO name
	| ALTER RULE name ON qualified_name RENAME TO name
	| ALTER TRIGGER name ON qualified_name RENAME TO name
	| ALTER EVENT TRIGGER name RENAME TO name
	| ALTER ROLE RoleId RENAME TO RoleId
	| ALTER USER RoleId RENAME TO RoleId
	| ALTER TABLESPACE name RENAME TO name
	| ALTER STATISTICS any_name RENAME TO name
	| ALTER TEXT_P SEARCH PARSER any_name RENAME TO name
	| ALTER TEXT_P SEARCH DICTIONARY any_name RENAME TO name
	| ALTER TEXT_P SEARCH TEMPLATE any_name RENAME TO name
	| ALTER TEXT_P SEARCH CONFIGURATION any_name RENAME TO name
	| ALTER TYPE_P any_name RENAME TO name
	| ALTER TYPE_P any_name RENAME ATTRIBUTE name TO name opt_drop_behavior
	;

opt_column :
	COLUMN
	| /*empty*/
	;

opt_set_data :
	SET /*1N*/ DATA_P
	| /*empty*/
	;

AlterObjectDependsStmt :
	ALTER FUNCTION function_with_argtypes opt_no DEPENDS ON EXTENSION name
	| ALTER PROCEDURE function_with_argtypes opt_no DEPENDS ON EXTENSION name
	| ALTER ROUTINE function_with_argtypes opt_no DEPENDS ON EXTENSION name
	| ALTER TRIGGER name ON qualified_name opt_no DEPENDS ON EXTENSION name
	| ALTER MATERIALIZED VIEW qualified_name opt_no DEPENDS ON EXTENSION name
	| ALTER INDEX qualified_name opt_no DEPENDS ON EXTENSION name
	;

opt_no :
	NO
	| /*empty*/
	;

AlterObjectSchemaStmt :
	ALTER AGGREGATE aggregate_with_argtypes SET /*1N*/ SCHEMA name
	| ALTER COLLATION any_name SET /*1N*/ SCHEMA name
	| ALTER CONVERSION_P any_name SET /*1N*/ SCHEMA name
	| ALTER DOMAIN_P any_name SET /*1N*/ SCHEMA name
	| ALTER EXTENSION name SET /*1N*/ SCHEMA name
	| ALTER FUNCTION function_with_argtypes SET /*1N*/ SCHEMA name
	| ALTER OPERATOR /*16L*/ operator_with_argtypes SET /*1N*/ SCHEMA name
	| ALTER OPERATOR /*16L*/ CLASS any_name USING name SET /*1N*/ SCHEMA name
	| ALTER OPERATOR /*16L*/ FAMILY any_name USING name SET /*1N*/ SCHEMA name
	| ALTER PROCEDURE function_with_argtypes SET /*1N*/ SCHEMA name
	| ALTER ROUTINE function_with_argtypes SET /*1N*/ SCHEMA name
	| ALTER TABLE relation_expr SET /*1N*/ SCHEMA name
	| ALTER TABLE IF_P EXISTS relation_expr SET /*1N*/ SCHEMA name
	| ALTER STATISTICS any_name SET /*1N*/ SCHEMA name
	| ALTER TEXT_P SEARCH PARSER any_name SET /*1N*/ SCHEMA name
	| ALTER TEXT_P SEARCH DICTIONARY any_name SET /*1N*/ SCHEMA name
	| ALTER TEXT_P SEARCH TEMPLATE any_name SET /*1N*/ SCHEMA name
	| ALTER TEXT_P SEARCH CONFIGURATION any_name SET /*1N*/ SCHEMA name
	| ALTER SEQUENCE qualified_name SET /*1N*/ SCHEMA name
	| ALTER SEQUENCE IF_P EXISTS qualified_name SET /*1N*/ SCHEMA name
	| ALTER VIEW qualified_name SET /*1N*/ SCHEMA name
	| ALTER VIEW IF_P EXISTS qualified_name SET /*1N*/ SCHEMA name
	| ALTER MATERIALIZED VIEW qualified_name SET /*1N*/ SCHEMA name
	| ALTER MATERIALIZED VIEW IF_P EXISTS qualified_name SET /*1N*/ SCHEMA name
	| ALTER FOREIGN TABLE relation_expr SET /*1N*/ SCHEMA name
	| ALTER FOREIGN TABLE IF_P EXISTS relation_expr SET /*1N*/ SCHEMA name
	| ALTER TYPE_P any_name SET /*1N*/ SCHEMA name
	;

AlterOperatorStmt :
	ALTER OPERATOR /*16L*/ operator_with_argtypes SET /*1N*/ '(' /*24L*/ operator_def_list ')' /*24L*/
	;

operator_def_list :
	operator_def_elem
	| operator_def_list ',' operator_def_elem
	;

operator_def_elem :
	ColLabel '=' /*8N*/ NONE
	| ColLabel '=' /*8N*/ operator_def_arg
	;

operator_def_arg :
	func_type
	| reserved_keyword
	| qual_all_Op
	| NumericOnly
	| Sconst
	;

AlterTypeStmt :
	ALTER TYPE_P any_name SET /*1N*/ '(' /*24L*/ operator_def_list ')' /*24L*/
	;

AlterOwnerStmt :
	ALTER AGGREGATE aggregate_with_argtypes OWNER TO RoleSpec
	| ALTER COLLATION any_name OWNER TO RoleSpec
	| ALTER CONVERSION_P any_name OWNER TO RoleSpec
	| ALTER DATABASE name OWNER TO RoleSpec
	| ALTER DOMAIN_P any_name OWNER TO RoleSpec
	| ALTER FUNCTION function_with_argtypes OWNER TO RoleSpec
	| ALTER opt_procedural LANGUAGE name OWNER TO RoleSpec
	| ALTER LARGE_P OBJECT_P /*12N*/ NumericOnly OWNER TO RoleSpec
	| ALTER OPERATOR /*16L*/ operator_with_argtypes OWNER TO RoleSpec
	| ALTER OPERATOR /*16L*/ CLASS any_name USING name OWNER TO RoleSpec
	| ALTER OPERATOR /*16L*/ FAMILY any_name USING name OWNER TO RoleSpec
	| ALTER PROCEDURE function_with_argtypes OWNER TO RoleSpec
	| ALTER ROUTINE function_with_argtypes OWNER TO RoleSpec
	| ALTER SCHEMA name OWNER TO RoleSpec
	| ALTER TYPE_P any_name OWNER TO RoleSpec
	| ALTER TABLESPACE name OWNER TO RoleSpec
	| ALTER STATISTICS any_name OWNER TO RoleSpec
	| ALTER TEXT_P SEARCH DICTIONARY any_name OWNER TO RoleSpec
	| ALTER TEXT_P SEARCH CONFIGURATION any_name OWNER TO RoleSpec
	| ALTER FOREIGN DATA_P WRAPPER name OWNER TO RoleSpec
	| ALTER SERVER name OWNER TO RoleSpec
	| ALTER EVENT TRIGGER name OWNER TO RoleSpec
	| ALTER PUBLICATION name OWNER TO RoleSpec
	| ALTER SUBSCRIPTION name OWNER TO RoleSpec
	;

CreatePublicationStmt :
	CREATE PUBLICATION name opt_definition
	| CREATE PUBLICATION name FOR ALL TABLES opt_definition
	| CREATE PUBLICATION name FOR pub_obj_list opt_definition
	;

PublicationObjSpec :
	TABLE relation_expr opt_column_list OptWhereClause
	| TABLES IN_P /*9N*/ SCHEMA ColId
	| TABLES IN_P /*9N*/ SCHEMA CURRENT_SCHEMA
	| ColId opt_column_list OptWhereClause
	| ColId indirection opt_column_list OptWhereClause
	| extended_relation_expr opt_column_list OptWhereClause
	| CURRENT_SCHEMA
	;

pub_obj_list :
	PublicationObjSpec
	| pub_obj_list ',' PublicationObjSpec
	;

AlterPublicationStmt :
	ALTER PUBLICATION name SET /*1N*/ definition
	| ALTER PUBLICATION name ADD_P pub_obj_list
	| ALTER PUBLICATION name SET /*1N*/ pub_obj_list
	| ALTER PUBLICATION name DROP pub_obj_list
	;

CreateSubscriptionStmt :
	CREATE SUBSCRIPTION name CONNECTION Sconst PUBLICATION name_list opt_definition
	;

AlterSubscriptionStmt :
	ALTER SUBSCRIPTION name SET /*1N*/ definition
	| ALTER SUBSCRIPTION name CONNECTION Sconst
	| ALTER SUBSCRIPTION name REFRESH PUBLICATION opt_definition
	| ALTER SUBSCRIPTION name ADD_P PUBLICATION name_list opt_definition
	| ALTER SUBSCRIPTION name DROP PUBLICATION name_list opt_definition
	| ALTER SUBSCRIPTION name SET /*1N*/ PUBLICATION name_list opt_definition
	| ALTER SUBSCRIPTION name ENABLE_P
	| ALTER SUBSCRIPTION name DISABLE_P
	| ALTER SUBSCRIPTION name SKIP definition
	;

DropSubscriptionStmt :
	DROP SUBSCRIPTION name opt_drop_behavior
	| DROP SUBSCRIPTION IF_P EXISTS name opt_drop_behavior
	;

RuleStmt :
	CREATE opt_or_replace RULE name AS ON event TO qualified_name where_clause DO opt_instead RuleActionList
	;

RuleActionList :
	NOTHING
	| RuleActionStmt
	| '(' /*24L*/ RuleActionMulti ')' /*24L*/
	;

RuleActionMulti :
	RuleActionMulti ';' RuleActionStmtOrEmpty
	| RuleActionStmtOrEmpty
	;

RuleActionStmt :
	SelectStmt
	| InsertStmt
	| UpdateStmt
	| DeleteStmt
	| NotifyStmt
	;

RuleActionStmtOrEmpty :
	RuleActionStmt
	| /*empty*/
	;

event :
	SELECT
	| UPDATE
	| DELETE_P
	| INSERT
	;

opt_instead :
	INSTEAD
	| ALSO
	| /*empty*/
	;

NotifyStmt :
	NOTIFY ColId notify_payload
	;

notify_payload :
	',' Sconst
	| /*empty*/
	;

ListenStmt :
	LISTEN ColId
	;

UnlistenStmt :
	UNLISTEN ColId
	| UNLISTEN '*' /*18L*/
	;

TransactionStmt :
	ABORT_P opt_transaction opt_transaction_chain
	| START TRANSACTION transaction_mode_list_or_empty
	| COMMIT opt_transaction opt_transaction_chain
	| ROLLBACK opt_transaction opt_transaction_chain
	| SAVEPOINT ColId
	| RELEASE SAVEPOINT ColId
	| RELEASE ColId
	| ROLLBACK opt_transaction TO SAVEPOINT ColId
	| ROLLBACK opt_transaction TO ColId
	| PREPARE TRANSACTION Sconst
	| COMMIT PREPARED Sconst
	| ROLLBACK PREPARED Sconst
	;

TransactionStmtLegacy :
	BEGIN_P opt_transaction transaction_mode_list_or_empty
	| END_P opt_transaction opt_transaction_chain
	;

opt_transaction :
	WORK
	| TRANSACTION
	| /*empty*/
	;

transaction_mode_item :
	ISOLATION LEVEL iso_level
	| READ ONLY
	| READ WRITE
	| DEFERRABLE
	| NOT /*6R*/ DEFERRABLE
	;

transaction_mode_list :
	transaction_mode_item
	| transaction_mode_list ',' transaction_mode_item
	| transaction_mode_list transaction_mode_item
	;

transaction_mode_list_or_empty :
	transaction_mode_list
	| /*empty*/
	;

opt_transaction_chain :
	AND /*5L*/ CHAIN
	| AND /*5L*/ NO CHAIN
	| /*empty*/
	;

ViewStmt :
	CREATE OptTemp VIEW qualified_name opt_column_list opt_reloptions AS SelectStmt opt_check_option
	| CREATE OR /*4L*/ REPLACE OptTemp VIEW qualified_name opt_column_list opt_reloptions AS SelectStmt opt_check_option
	| CREATE OptTemp RECURSIVE VIEW qualified_name '(' /*24L*/ columnList ')' /*24L*/ opt_reloptions AS SelectStmt opt_check_option
	| CREATE OR /*4L*/ REPLACE OptTemp RECURSIVE VIEW qualified_name '(' /*24L*/ columnList ')' /*24L*/ opt_reloptions AS SelectStmt opt_check_option
	;

opt_check_option :
	WITH /*13N*/ CHECK OPTION
	| WITH /*13N*/ CASCADED CHECK OPTION
	| WITH /*13N*/ LOCAL CHECK OPTION
	| /*empty*/
	;

LoadStmt :
	LOAD file_name
	;

CreatedbStmt :
	CREATE DATABASE name opt_with createdb_opt_list
	;

createdb_opt_list :
	createdb_opt_items
	| /*empty*/
	;

createdb_opt_items :
	createdb_opt_item
	| createdb_opt_items createdb_opt_item
	;

createdb_opt_item :
	createdb_opt_name opt_equal NumericOnly
	| createdb_opt_name opt_equal opt_boolean_or_string
	| createdb_opt_name opt_equal DEFAULT
	;

createdb_opt_name :
	IDENT /*15N*/
	| CONNECTION LIMIT
	| ENCODING
	| LOCATION
	| OWNER
	| TABLESPACE
	| TEMPLATE
	;

opt_equal :
	'=' /*8N*/
	| /*empty*/
	;

AlterDatabaseStmt :
	ALTER DATABASE name WITH /*13N*/ createdb_opt_list
	| ALTER DATABASE name createdb_opt_list
	| ALTER DATABASE name SET /*1N*/ TABLESPACE name
	| ALTER DATABASE name REFRESH COLLATION VERSION_P
	;

AlterDatabaseSetStmt :
	ALTER DATABASE name SetResetClause
	;

DropdbStmt :
	DROP DATABASE name
	| DROP DATABASE IF_P EXISTS name
	| DROP DATABASE name opt_with '(' /*24L*/ drop_option_list ')' /*24L*/
	| DROP DATABASE IF_P EXISTS name opt_with '(' /*24L*/ drop_option_list ')' /*24L*/
	;

drop_option_list :
	drop_option
	| drop_option_list ',' drop_option
	;

drop_option :
	FORCE
	;

AlterCollationStmt :
	ALTER COLLATION any_name REFRESH VERSION_P
	;

AlterSystemStmt :
	ALTER SYSTEM_P SET /*1N*/ generic_set
	| ALTER SYSTEM_P RESET generic_reset
	;

CreateDomainStmt :
	CREATE DOMAIN_P any_name opt_as Typename ColQualList
	;

AlterDomainStmt :
	ALTER DOMAIN_P any_name alter_column_default
	| ALTER DOMAIN_P any_name DROP NOT /*6R*/ NULL_P
	| ALTER DOMAIN_P any_name SET /*1N*/ NOT /*6R*/ NULL_P
	| ALTER DOMAIN_P any_name ADD_P TableConstraint
	| ALTER DOMAIN_P any_name DROP CONSTRAINT name opt_drop_behavior
	| ALTER DOMAIN_P any_name DROP CONSTRAINT IF_P EXISTS name opt_drop_behavior
	| ALTER DOMAIN_P any_name VALIDATE CONSTRAINT name
	;

opt_as :
	AS
	| /*empty*/
	;

AlterTSDictionaryStmt :
	ALTER TEXT_P SEARCH DICTIONARY any_name definition
	;

AlterTSConfigurationStmt :
	ALTER TEXT_P SEARCH CONFIGURATION any_name ADD_P MAPPING FOR name_list any_with any_name_list
	| ALTER TEXT_P SEARCH CONFIGURATION any_name ALTER MAPPING FOR name_list any_with any_name_list
	| ALTER TEXT_P SEARCH CONFIGURATION any_name ALTER MAPPING REPLACE any_name any_with any_name
	| ALTER TEXT_P SEARCH CONFIGURATION any_name ALTER MAPPING FOR name_list REPLACE any_name any_with any_name
	| ALTER TEXT_P SEARCH CONFIGURATION any_name DROP MAPPING FOR name_list
	| ALTER TEXT_P SEARCH CONFIGURATION any_name DROP MAPPING IF_P EXISTS FOR name_list
	;

any_with :
	WITH /*13N*/
	| WITH_LA
	;

CreateConversionStmt :
	CREATE opt_default CONVERSION_P any_name FOR Sconst TO Sconst FROM any_name
	;

ClusterStmt :
	CLUSTER opt_verbose qualified_name cluster_index_specification
	| CLUSTER '(' /*24L*/ utility_option_list ')' /*24L*/ qualified_name cluster_index_specification
	| CLUSTER opt_verbose
	| CLUSTER opt_verbose name ON qualified_name
	;

cluster_index_specification :
	USING name
	| /*empty*/
	;

VacuumStmt :
	VACUUM opt_full opt_freeze opt_verbose opt_analyze opt_vacuum_relation_list
	| VACUUM '(' /*24L*/ utility_option_list ')' /*24L*/ opt_vacuum_relation_list
	;

AnalyzeStmt :
	analyze_keyword opt_verbose opt_vacuum_relation_list
	| analyze_keyword '(' /*24L*/ utility_option_list ')' /*24L*/ opt_vacuum_relation_list
	;

utility_option_list :
	utility_option_elem
	| utility_option_list ',' utility_option_elem
	;

analyze_keyword :
	ANALYZE
	| ANALYSE
	;

utility_option_elem :
	utility_option_name utility_option_arg
	;

utility_option_name :
	NonReservedWord
	| analyze_keyword
	| FORMAT_LA
	;

utility_option_arg :
	opt_boolean_or_string
	| NumericOnly
	| /*empty*/
	;

opt_analyze :
	analyze_keyword
	| /*empty*/
	;

opt_verbose :
	VERBOSE
	| /*empty*/
	;

opt_full :
	FULL /*27L*/
	| /*empty*/
	;

opt_freeze :
	FREEZE
	| /*empty*/
	;

opt_name_list :
	'(' /*24L*/ name_list ')' /*24L*/
	| /*empty*/
	;

vacuum_relation :
	qualified_name opt_name_list
	;

vacuum_relation_list :
	vacuum_relation
	| vacuum_relation_list ',' vacuum_relation
	;

opt_vacuum_relation_list :
	vacuum_relation_list
	| /*empty*/
	;

ExplainStmt :
	EXPLAIN ExplainableStmt
	| EXPLAIN analyze_keyword opt_verbose ExplainableStmt
	| EXPLAIN VERBOSE ExplainableStmt
	| EXPLAIN '(' /*24L*/ utility_option_list ')' /*24L*/ ExplainableStmt
	;

ExplainableStmt :
	SelectStmt
	| InsertStmt
	| UpdateStmt
	| DeleteStmt
	| MergeStmt
	| DeclareCursorStmt
	| CreateAsStmt
	| CreateMatViewStmt
	| RefreshMatViewStmt
	| ExecuteStmt
	;

PrepareStmt :
	PREPARE name prep_type_clause AS PreparableStmt
	;

prep_type_clause :
	'(' /*24L*/ type_list ')' /*24L*/
	| /*empty*/
	;

PreparableStmt :
	SelectStmt
	| InsertStmt
	| UpdateStmt
	| DeleteStmt
	| MergeStmt
	;

ExecuteStmt :
	EXECUTE name execute_param_clause
	| CREATE OptTemp TABLE create_as_target AS EXECUTE name execute_param_clause opt_with_data
	| CREATE OptTemp TABLE IF_P NOT /*6R*/ EXISTS create_as_target AS EXECUTE name execute_param_clause opt_with_data
	;

execute_param_clause :
	'(' /*24L*/ expr_list ')' /*24L*/
	| /*empty*/
	;

DeallocateStmt :
	DEALLOCATE name
	| DEALLOCATE PREPARE name
	| DEALLOCATE ALL
	| DEALLOCATE PREPARE ALL
	;

InsertStmt :
	opt_with_clause INSERT INTO insert_target insert_rest opt_on_conflict returning_clause
	;

insert_target :
	qualified_name
	| qualified_name AS ColId
	;

insert_rest :
	SelectStmt
	| OVERRIDING override_kind VALUE_P /*12N*/ SelectStmt
	| '(' /*24L*/ insert_column_list ')' /*24L*/ SelectStmt
	| '(' /*24L*/ insert_column_list ')' /*24L*/ OVERRIDING override_kind VALUE_P /*12N*/ SelectStmt
	| DEFAULT VALUES
	;

override_kind :
	USER
	| SYSTEM_P
	;

insert_column_list :
	insert_column_item
	| insert_column_list ',' insert_column_item
	;

insert_column_item :
	ColId opt_indirection
	;

opt_on_conflict :
	ON CONFLICT opt_conf_expr DO UPDATE SET /*1N*/ set_clause_list where_clause
	| ON CONFLICT opt_conf_expr DO NOTHING
	| /*empty*/
	;

opt_conf_expr :
	'(' /*24L*/ index_params ')' /*24L*/ where_clause
	| ON CONSTRAINT name
	| /*empty*/
	;

returning_clause :
	RETURNING target_list
	| /*empty*/
	;

DeleteStmt :
	opt_with_clause DELETE_P FROM relation_expr_opt_alias using_clause where_or_current_clause returning_clause
	;

using_clause :
	USING from_list
	| /*empty*/
	;

LockStmt :
	LOCK_P opt_table relation_expr_list opt_lock opt_nowait
	;

opt_lock :
	IN_P /*9N*/ lock_type MODE
	| /*empty*/
	;

lock_type :
	ACCESS SHARE
	| ROW SHARE
	| ROW EXCLUSIVE
	| SHARE UPDATE EXCLUSIVE
	| SHARE
	| SHARE ROW EXCLUSIVE
	| EXCLUSIVE
	| ACCESS EXCLUSIVE
	;

opt_nowait :
	NOWAIT
	| /*empty*/
	;

opt_nowait_or_skip :
	NOWAIT
	| SKIP LOCKED
	| /*empty*/
	;

UpdateStmt :
	opt_with_clause UPDATE relation_expr_opt_alias SET /*1N*/ set_clause_list from_clause where_or_current_clause returning_clause
	;

set_clause_list :
	set_clause
	| set_clause_list ',' set_clause
	;

set_clause :
	set_target '=' /*8N*/ a_expr
	| '(' /*24L*/ set_target_list ')' /*24L*/ '=' /*8N*/ a_expr
	;

set_target :
	ColId opt_indirection
	;

set_target_list :
	set_target
	| set_target_list ',' set_target
	;

MergeStmt :
	opt_with_clause MERGE INTO relation_expr_opt_alias USING table_ref ON a_expr merge_when_list
	;

merge_when_list :
	merge_when_clause
	| merge_when_list merge_when_clause
	;

merge_when_clause :
	WHEN MATCHED opt_merge_when_condition THEN merge_update
	| WHEN MATCHED opt_merge_when_condition THEN merge_delete
	| WHEN NOT /*6R*/ MATCHED opt_merge_when_condition THEN merge_insert
	| WHEN MATCHED opt_merge_when_condition THEN DO NOTHING
	| WHEN NOT /*6R*/ MATCHED opt_merge_when_condition THEN DO NOTHING
	;

opt_merge_when_condition :
	AND /*5L*/ a_expr
	| /*empty*/
	;

merge_update :
	UPDATE SET /*1N*/ set_clause_list
	;

merge_delete :
	DELETE_P
	;

merge_insert :
	INSERT merge_values_clause
	| INSERT OVERRIDING override_kind VALUE_P /*12N*/ merge_values_clause
	| INSERT '(' /*24L*/ insert_column_list ')' /*24L*/ merge_values_clause
	| INSERT '(' /*24L*/ insert_column_list ')' /*24L*/ OVERRIDING override_kind VALUE_P /*12N*/ merge_values_clause
	| INSERT DEFAULT VALUES
	;

merge_values_clause :
	VALUES '(' /*24L*/ expr_list ')' /*24L*/
	;

DeclareCursorStmt :
	DECLARE cursor_name cursor_options CURSOR opt_hold FOR SelectStmt
	;

cursor_name :
	name
	;

cursor_options :
	/*empty*/
	| cursor_options NO SCROLL
	| cursor_options SCROLL
	| cursor_options BINARY
	| cursor_options ASENSITIVE
	| cursor_options INSENSITIVE
	;

opt_hold :
	/*empty*/
	| WITH /*13N*/ HOLD
	| WITHOUT /*13N*/ HOLD
	;

SelectStmt :
	select_no_parens %prec UMINUS /*22R*/
	| select_with_parens %prec UMINUS /*22R*/
	;

select_with_parens :
	'(' /*24L*/ select_no_parens ')' /*24L*/
	| '(' /*24L*/ select_with_parens ')' /*24L*/
	;

select_no_parens :
	simple_select
	| select_clause sort_clause
	| select_clause opt_sort_clause for_locking_clause opt_select_limit
	| select_clause opt_sort_clause select_limit opt_for_locking_clause
	| with_clause select_clause
	| with_clause select_clause sort_clause
	| with_clause select_clause opt_sort_clause for_locking_clause opt_select_limit
	| with_clause select_clause opt_sort_clause select_limit opt_for_locking_clause
	;

select_clause :
	simple_select
	| select_with_parens
	;

simple_select :
	SELECT opt_all_clause opt_target_list into_clause from_clause where_clause group_clause having_clause window_clause
	| SELECT distinct_clause target_list into_clause from_clause where_clause group_clause having_clause window_clause
	| values_clause
	| TABLE relation_expr
	| select_clause UNION /*2L*/ set_quantifier select_clause
	| select_clause INTERSECT /*3L*/ set_quantifier select_clause
	| select_clause EXCEPT /*2L*/ set_quantifier select_clause
	;

with_clause :
	WITH /*13N*/ cte_list
	| WITH_LA cte_list
	| WITH /*13N*/ RECURSIVE cte_list
	;

cte_list :
	common_table_expr
	| cte_list ',' common_table_expr
	;

common_table_expr :
	name opt_name_list AS opt_materialized '(' /*24L*/ PreparableStmt ')' /*24L*/ opt_search_clause opt_cycle_clause
	;

opt_materialized :
	MATERIALIZED
	| NOT /*6R*/ MATERIALIZED
	| /*empty*/
	;

opt_search_clause :
	SEARCH DEPTH FIRST_P BY columnList SET /*1N*/ ColId
	| SEARCH BREADTH FIRST_P BY columnList SET /*1N*/ ColId
	| /*empty*/
	;

opt_cycle_clause :
	CYCLE columnList SET /*1N*/ ColId TO AexprConst DEFAULT AexprConst USING ColId
	| CYCLE columnList SET /*1N*/ ColId USING ColId
	| /*empty*/
	;

opt_with_clause :
	with_clause
	| /*empty*/
	;

into_clause :
	INTO OptTempTableName
	| /*empty*/
	;

OptTempTableName :
	TEMPORARY opt_table qualified_name
	| TEMP opt_table qualified_name
	| LOCAL TEMPORARY opt_table qualified_name
	| LOCAL TEMP opt_table qualified_name
	| GLOBAL TEMPORARY opt_table qualified_name
	| GLOBAL TEMP opt_table qualified_name
	| UNLOGGED opt_table qualified_name
	| TABLE qualified_name
	| qualified_name
	;

opt_table :
	TABLE
	| /*empty*/
	;

set_quantifier :
	ALL
	| DISTINCT
	| /*empty*/
	;

distinct_clause :
	DISTINCT
	| DISTINCT ON '(' /*24L*/ expr_list ')' /*24L*/
	;

opt_all_clause :
	ALL
	| /*empty*/
	;

opt_distinct_clause :
	distinct_clause
	| opt_all_clause
	;

opt_sort_clause :
	sort_clause
	| /*empty*/
	;

sort_clause :
	ORDER BY sortby_list
	;

sortby_list :
	sortby
	| sortby_list ',' sortby
	;

sortby :
	a_expr USING qual_all_Op opt_nulls_order
	| a_expr opt_asc_desc opt_nulls_order
	;

select_limit :
	limit_clause offset_clause
	| offset_clause limit_clause
	| limit_clause
	| offset_clause
	;

opt_select_limit :
	select_limit
	| /*empty*/
	;

limit_clause :
	LIMIT select_limit_value
	| LIMIT select_limit_value ',' select_offset_value
	| FETCH first_or_next select_fetch_first_value row_or_rows ONLY
	| FETCH first_or_next select_fetch_first_value row_or_rows WITH /*13N*/ TIES
	| FETCH first_or_next row_or_rows ONLY
	| FETCH first_or_next row_or_rows WITH /*13N*/ TIES
	;

offset_clause :
	OFFSET select_offset_value
	| OFFSET select_fetch_first_value row_or_rows
	;

select_limit_value :
	a_expr
	| ALL
	;

select_offset_value :
	a_expr
	;

select_fetch_first_value :
	c_expr
	| '+' /*17L*/ I_or_F_const
	| '-' /*17L*/ I_or_F_const
	;

I_or_F_const :
	Iconst
	| FCONST
	;

row_or_rows :
	ROW
	| ROWS /*15N*/
	;

first_or_next :
	FIRST_P
	| NEXT
	;

group_clause :
	GROUP_P BY set_quantifier group_by_list
	| /*empty*/
	;

group_by_list :
	group_by_item
	| group_by_list ',' group_by_item
	;

group_by_item :
	a_expr
	| empty_grouping_set
	| cube_clause
	| rollup_clause
	| grouping_sets_clause
	;

empty_grouping_set :
	'(' /*24L*/ ')' /*24L*/
	;

rollup_clause :
	ROLLUP /*15N*/ '(' /*24L*/ expr_list ')' /*24L*/
	;

cube_clause :
	CUBE /*15N*/ '(' /*24L*/ expr_list ')' /*24L*/
	;

grouping_sets_clause :
	GROUPING SETS '(' /*24L*/ group_by_list ')' /*24L*/
	;

having_clause :
	HAVING a_expr
	| /*empty*/
	;

for_locking_clause :
	for_locking_items
	| FOR READ ONLY
	;

opt_for_locking_clause :
	for_locking_clause
	| /*empty*/
	;

for_locking_items :
	for_locking_item
	| for_locking_items for_locking_item
	;

for_locking_item :
	for_locking_strength locked_rels_list opt_nowait_or_skip
	;

for_locking_strength :
	FOR UPDATE
	| FOR NO KEY UPDATE
	| FOR SHARE
	| FOR KEY SHARE
	;

locked_rels_list :
	OF qualified_name_list
	| /*empty*/
	;

values_clause :
	VALUES '(' /*24L*/ expr_list ')' /*24L*/
	| values_clause ',' '(' /*24L*/ expr_list ')' /*24L*/
	;

from_clause :
	FROM from_list
	| /*empty*/
	;

from_list :
	table_ref
	| from_list ',' table_ref
	;

table_ref :
	relation_expr opt_alias_clause
	| relation_expr opt_alias_clause tablesample_clause
	| func_table func_alias_clause
	| LATERAL_P func_table func_alias_clause
	| xmltable opt_alias_clause
	| LATERAL_P xmltable opt_alias_clause
	| select_with_parens opt_alias_clause
	| LATERAL_P select_with_parens opt_alias_clause
	| joined_table
	| '(' /*24L*/ joined_table ')' /*24L*/ alias_clause
	;

joined_table :
	'(' /*24L*/ joined_table ')' /*24L*/
	| table_ref CROSS /*27L*/ JOIN /*27L*/ table_ref
	| table_ref join_type JOIN /*27L*/ table_ref join_qual
	| table_ref JOIN /*27L*/ table_ref join_qual
	| table_ref NATURAL /*27L*/ join_type JOIN /*27L*/ table_ref
	| table_ref NATURAL /*27L*/ JOIN /*27L*/ table_ref
	;

alias_clause :
	AS ColId '(' /*24L*/ name_list ')' /*24L*/
	| AS ColId
	| ColId '(' /*24L*/ name_list ')' /*24L*/
	| ColId
	;

opt_alias_clause :
	alias_clause
	| /*empty*/
	;

opt_alias_clause_for_join_using :
	AS ColId
	| /*empty*/
	;

func_alias_clause :
	alias_clause
	| AS '(' /*24L*/ TableFuncElementList ')' /*24L*/
	| AS ColId '(' /*24L*/ TableFuncElementList ')' /*24L*/
	| ColId '(' /*24L*/ TableFuncElementList ')' /*24L*/
	| /*empty*/
	;

join_type :
	FULL /*27L*/ opt_outer
	| LEFT /*27L*/ opt_outer
	| RIGHT /*27L*/ opt_outer
	| INNER_P /*27L*/
	;

opt_outer :
	OUTER_P
	| /*empty*/
	;

join_qual :
	USING '(' /*24L*/ name_list ')' /*24L*/ opt_alias_clause_for_join_using
	| ON a_expr
	;

relation_expr :
	qualified_name
	| extended_relation_expr
	;

extended_relation_expr :
	qualified_name '*' /*18L*/
	| ONLY qualified_name
	| ONLY '(' /*24L*/ qualified_name ')' /*24L*/
	;

relation_expr_list :
	relation_expr
	| relation_expr_list ',' relation_expr
	;

relation_expr_opt_alias :
	relation_expr %prec UMINUS /*22R*/
	| relation_expr ColId
	| relation_expr AS ColId
	;

tablesample_clause :
	TABLESAMPLE func_name '(' /*24L*/ expr_list ')' /*24L*/ opt_repeatable_clause
	;

opt_repeatable_clause :
	REPEATABLE '(' /*24L*/ a_expr ')' /*24L*/
	| /*empty*/
	;

func_table :
	func_expr_windowless opt_ordinality
	| ROWS /*15N*/ FROM '(' /*24L*/ rowsfrom_list ')' /*24L*/ opt_ordinality
	;

rowsfrom_item :
	func_expr_windowless opt_col_def_list
	;

rowsfrom_list :
	rowsfrom_item
	| rowsfrom_list ',' rowsfrom_item
	;

opt_col_def_list :
	AS '(' /*24L*/ TableFuncElementList ')' /*24L*/
	| /*empty*/
	;

opt_ordinality :
	WITH_LA ORDINALITY
	| /*empty*/
	;

where_clause :
	WHERE a_expr
	| /*empty*/
	;

where_or_current_clause :
	WHERE a_expr
	| WHERE CURRENT_P OF cursor_name
	| /*empty*/
	;

OptTableFuncElementList :
	TableFuncElementList
	| /*empty*/
	;

TableFuncElementList :
	TableFuncElement
	| TableFuncElementList ',' TableFuncElement
	;

TableFuncElement :
	ColId Typename opt_collate_clause
	;

xmltable :
	XMLTABLE '(' /*24L*/ c_expr xmlexists_argument COLUMNS xmltable_column_list ')' /*24L*/
	| XMLTABLE '(' /*24L*/ XMLNAMESPACES '(' /*24L*/ xml_namespace_list ')' /*24L*/ ',' c_expr xmlexists_argument COLUMNS xmltable_column_list ')' /*24L*/
	;

xmltable_column_list :
	xmltable_column_el
	| xmltable_column_list ',' xmltable_column_el
	;

xmltable_column_el :
	ColId Typename
	| ColId Typename xmltable_column_option_list
	| ColId FOR ORDINALITY
	;

xmltable_column_option_list :
	xmltable_column_option_el
	| xmltable_column_option_list xmltable_column_option_el
	;

xmltable_column_option_el :
	IDENT /*15N*/ b_expr
	| DEFAULT b_expr
	| NOT /*6R*/ NULL_P
	| NULL_P
	;

xml_namespace_list :
	xml_namespace_el
	| xml_namespace_list ',' xml_namespace_el
	;

xml_namespace_el :
	b_expr AS ColLabel
	| DEFAULT b_expr
	;

Typename :
	SimpleTypename opt_array_bounds
	| SETOF SimpleTypename opt_array_bounds
	| SimpleTypename ARRAY '[' /*23L*/ Iconst ']' /*23L*/
	| SETOF SimpleTypename ARRAY '[' /*23L*/ Iconst ']' /*23L*/
	| SimpleTypename ARRAY
	| SETOF SimpleTypename ARRAY
	;

opt_array_bounds :
	opt_array_bounds '[' /*23L*/ ']' /*23L*/
	| opt_array_bounds '[' /*23L*/ Iconst ']' /*23L*/
	| /*empty*/
	;

SimpleTypename :
	GenericType
	| Numeric
	| Bit
	| Character
	| ConstDatetime
	| ConstInterval opt_interval
	| ConstInterval '(' /*24L*/ Iconst ')' /*24L*/
	;

ConstTypename :
	Numeric
	| ConstBit
	| ConstCharacter
	| ConstDatetime
	;

GenericType :
	type_function_name opt_type_modifiers
	| type_function_name attrs opt_type_modifiers
	;

opt_type_modifiers :
	'(' /*24L*/ expr_list ')' /*24L*/
	| /*empty*/
	;

Numeric :
	INT_P
	| INTEGER
	| SMALLINT
	| BIGINT
	| REAL
	| FLOAT_P opt_float
	| DOUBLE_P PRECISION
	| DECIMAL_P opt_type_modifiers
	| DEC opt_type_modifiers
	| NUMERIC opt_type_modifiers
	| BOOLEAN_P
	;

opt_float :
	'(' /*24L*/ Iconst ')' /*24L*/
	| /*empty*/
	;

Bit :
	BitWithLength
	| BitWithoutLength
	;

ConstBit :
	BitWithLength
	| BitWithoutLength
	;

BitWithLength :
	BIT opt_varying '(' /*24L*/ expr_list ')' /*24L*/
	;

BitWithoutLength :
	BIT opt_varying
	;

Character :
	CharacterWithLength
	| CharacterWithoutLength
	;

ConstCharacter :
	CharacterWithLength
	| CharacterWithoutLength
	;

CharacterWithLength :
	character '(' /*24L*/ Iconst ')' /*24L*/
	;

CharacterWithoutLength :
	character
	;

character :
	CHARACTER opt_varying
	| CHAR_P opt_varying
	| VARCHAR
	| NATIONAL CHARACTER opt_varying
	| NATIONAL CHAR_P opt_varying
	| NCHAR opt_varying
	;

opt_varying :
	VARYING
	| /*empty*/
	;

ConstDatetime :
	TIMESTAMP '(' /*24L*/ Iconst ')' /*24L*/ opt_timezone
	| TIMESTAMP opt_timezone
	| TIME '(' /*24L*/ Iconst ')' /*24L*/ opt_timezone
	| TIME opt_timezone
	;

ConstInterval :
	INTERVAL
	;

opt_timezone :
	WITH_LA TIME ZONE
	| WITHOUT_LA TIME ZONE
	| /*empty*/
	;

opt_interval :
	YEAR_P
	| MONTH_P
	| DAY_P
	| HOUR_P
	| MINUTE_P
	| interval_second
	| YEAR_P TO MONTH_P
	| DAY_P TO HOUR_P
	| DAY_P TO MINUTE_P
	| DAY_P TO interval_second
	| HOUR_P TO MINUTE_P
	| HOUR_P TO interval_second
	| MINUTE_P TO interval_second
	| /*empty*/
	;

interval_second :
	SECOND_P
	| SECOND_P '(' /*24L*/ Iconst ')' /*24L*/
	;

a_expr :
	c_expr
	| a_expr TYPECAST /*25L*/ Typename
	| a_expr COLLATE /*21L*/ any_name
	| a_expr AT /*20L*/ TIME ZONE a_expr %prec AT /*20L*/
	| '+' /*17L*/ a_expr %prec UMINUS /*22R*/
	| '-' /*17L*/ a_expr %prec UMINUS /*22R*/
	| a_expr '+' /*17L*/ a_expr
	| a_expr '-' /*17L*/ a_expr
	| a_expr '*' /*18L*/ a_expr
	| a_expr '/' /*18L*/ a_expr
	| a_expr '%' /*18L*/ a_expr
	| a_expr '^' /*19L*/ a_expr
	| a_expr '<' /*8N*/ a_expr
	| a_expr '>' /*8N*/ a_expr
	| a_expr '=' /*8N*/ a_expr
	| a_expr LESS_EQUALS /*8N*/ a_expr
	| a_expr GREATER_EQUALS /*8N*/ a_expr
	| a_expr NOT_EQUALS /*8N*/ a_expr
	| a_expr qual_Op a_expr %prec Op /*16L*/
	| qual_Op a_expr %prec Op /*16L*/
	| a_expr AND /*5L*/ a_expr
	| a_expr OR /*4L*/ a_expr
	| NOT /*6R*/ a_expr
	| NOT_LA /*9N*/ a_expr %prec NOT /*6R*/
	| a_expr LIKE /*9N*/ a_expr
	| a_expr LIKE /*9N*/ a_expr ESCAPE /*10N*/ a_expr %prec LIKE /*9N*/
	| a_expr NOT_LA /*9N*/ LIKE /*9N*/ a_expr %prec NOT_LA /*9N*/
	| a_expr NOT_LA /*9N*/ LIKE /*9N*/ a_expr ESCAPE /*10N*/ a_expr %prec NOT_LA /*9N*/
	| a_expr ILIKE /*9N*/ a_expr
	| a_expr ILIKE /*9N*/ a_expr ESCAPE /*10N*/ a_expr %prec ILIKE /*9N*/
	| a_expr NOT_LA /*9N*/ ILIKE /*9N*/ a_expr %prec NOT_LA /*9N*/
	| a_expr NOT_LA /*9N*/ ILIKE /*9N*/ a_expr ESCAPE /*10N*/ a_expr %prec NOT_LA /*9N*/
	| a_expr SIMILAR /*9N*/ TO a_expr %prec SIMILAR /*9N*/
	| a_expr SIMILAR /*9N*/ TO a_expr ESCAPE /*10N*/ a_expr %prec SIMILAR /*9N*/
	| a_expr NOT_LA /*9N*/ SIMILAR /*9N*/ TO a_expr %prec NOT_LA /*9N*/
	| a_expr NOT_LA /*9N*/ SIMILAR /*9N*/ TO a_expr ESCAPE /*10N*/ a_expr %prec NOT_LA /*9N*/
	| a_expr IS /*7N*/ NULL_P %prec IS /*7N*/
	| a_expr ISNULL /*7N*/
	| a_expr IS /*7N*/ NOT /*6R*/ NULL_P %prec IS /*7N*/
	| a_expr NOTNULL /*7N*/
	| row OVERLAPS row
	| a_expr IS /*7N*/ TRUE_P %prec IS /*7N*/
	| a_expr IS /*7N*/ NOT /*6R*/ TRUE_P %prec IS /*7N*/
	| a_expr IS /*7N*/ FALSE_P %prec IS /*7N*/
	| a_expr IS /*7N*/ NOT /*6R*/ FALSE_P %prec IS /*7N*/
	| a_expr IS /*7N*/ UNKNOWN %prec IS /*7N*/
	| a_expr IS /*7N*/ NOT /*6R*/ UNKNOWN %prec IS /*7N*/
	| a_expr IS /*7N*/ DISTINCT FROM a_expr %prec IS /*7N*/
	| a_expr IS /*7N*/ NOT /*6R*/ DISTINCT FROM a_expr %prec IS /*7N*/
	| a_expr BETWEEN /*9N*/ opt_asymmetric b_expr AND /*5L*/ a_expr %prec BETWEEN /*9N*/
	| a_expr NOT_LA /*9N*/ BETWEEN /*9N*/ opt_asymmetric b_expr AND /*5L*/ a_expr %prec NOT_LA /*9N*/
	| a_expr BETWEEN /*9N*/ SYMMETRIC b_expr AND /*5L*/ a_expr %prec BETWEEN /*9N*/
	| a_expr NOT_LA /*9N*/ BETWEEN /*9N*/ SYMMETRIC b_expr AND /*5L*/ a_expr %prec NOT_LA /*9N*/
	| a_expr IN_P /*9N*/ in_expr
	| a_expr NOT_LA /*9N*/ IN_P /*9N*/ in_expr %prec NOT_LA /*9N*/
	| a_expr subquery_Op sub_type select_with_parens %prec Op /*16L*/
	| a_expr subquery_Op sub_type '(' /*24L*/ a_expr ')' /*24L*/ %prec Op /*16L*/
	| UNIQUE /*11N*/ opt_unique_null_treatment select_with_parens
	| a_expr IS /*7N*/ DOCUMENT_P %prec IS /*7N*/
	| a_expr IS /*7N*/ NOT /*6R*/ DOCUMENT_P %prec IS /*7N*/
	| a_expr IS /*7N*/ NORMALIZED %prec IS /*7N*/
	| a_expr IS /*7N*/ unicode_normal_form NORMALIZED %prec IS /*7N*/
	| a_expr IS /*7N*/ NOT /*6R*/ NORMALIZED %prec IS /*7N*/
	| a_expr IS /*7N*/ NOT /*6R*/ unicode_normal_form NORMALIZED %prec IS /*7N*/
	| a_expr IS /*7N*/ json_predicate_type_constraint json_key_uniqueness_constraint_opt %prec IS /*7N*/
	| a_expr IS /*7N*/ NOT /*6R*/ json_predicate_type_constraint json_key_uniqueness_constraint_opt %prec IS /*7N*/
	| DEFAULT
	;

b_expr :
	c_expr
	| b_expr TYPECAST /*25L*/ Typename
	| '+' /*17L*/ b_expr %prec UMINUS /*22R*/
	| '-' /*17L*/ b_expr %prec UMINUS /*22R*/
	| b_expr '+' /*17L*/ b_expr
	| b_expr '-' /*17L*/ b_expr
	| b_expr '*' /*18L*/ b_expr
	| b_expr '/' /*18L*/ b_expr
	| b_expr '%' /*18L*/ b_expr
	| b_expr '^' /*19L*/ b_expr
	| b_expr '<' /*8N*/ b_expr
	| b_expr '>' /*8N*/ b_expr
	| b_expr '=' /*8N*/ b_expr
	| b_expr LESS_EQUALS /*8N*/ b_expr
	| b_expr GREATER_EQUALS /*8N*/ b_expr
	| b_expr NOT_EQUALS /*8N*/ b_expr
	| b_expr qual_Op b_expr %prec Op /*16L*/
	| qual_Op b_expr %prec Op /*16L*/
	| b_expr IS /*7N*/ DISTINCT FROM b_expr %prec IS /*7N*/
	| b_expr IS /*7N*/ NOT /*6R*/ DISTINCT FROM b_expr %prec IS /*7N*/
	| b_expr IS /*7N*/ DOCUMENT_P %prec IS /*7N*/
	| b_expr IS /*7N*/ NOT /*6R*/ DOCUMENT_P %prec IS /*7N*/
	;

c_expr :
	columnref
	| AexprConst
	| PARAM opt_indirection
	| '(' /*24L*/ a_expr ')' /*24L*/ opt_indirection
	| case_expr
	| func_expr
	| select_with_parens %prec UMINUS /*22R*/
	| select_with_parens indirection
	| EXISTS select_with_parens
	| ARRAY select_with_parens
	| ARRAY array_expr
	| explicit_row
	| implicit_row
	| GROUPING '(' /*24L*/ expr_list ')' /*24L*/
	;

func_application :
	func_name '(' /*24L*/ ')' /*24L*/
	| func_name '(' /*24L*/ func_arg_list opt_sort_clause ')' /*24L*/
	| func_name '(' /*24L*/ VARIADIC func_arg_expr opt_sort_clause ')' /*24L*/
	| func_name '(' /*24L*/ func_arg_list ',' VARIADIC func_arg_expr opt_sort_clause ')' /*24L*/
	| func_name '(' /*24L*/ ALL func_arg_list opt_sort_clause ')' /*24L*/
	| func_name '(' /*24L*/ DISTINCT func_arg_list opt_sort_clause ')' /*24L*/
	| func_name '(' /*24L*/ '*' /*18L*/ ')' /*24L*/
	;

func_expr :
	func_application within_group_clause filter_clause over_clause
	| json_aggregate_func filter_clause over_clause
	| func_expr_common_subexpr
	;

func_expr_windowless :
	func_application
	| func_expr_common_subexpr
	| json_aggregate_func
	;

func_expr_common_subexpr :
	COLLATION FOR '(' /*24L*/ a_expr ')' /*24L*/
	| CURRENT_DATE
	| CURRENT_TIME
	| CURRENT_TIME '(' /*24L*/ Iconst ')' /*24L*/
	| CURRENT_TIMESTAMP
	| CURRENT_TIMESTAMP '(' /*24L*/ Iconst ')' /*24L*/
	| LOCALTIME
	| LOCALTIME '(' /*24L*/ Iconst ')' /*24L*/
	| LOCALTIMESTAMP
	| LOCALTIMESTAMP '(' /*24L*/ Iconst ')' /*24L*/
	| CURRENT_ROLE
	| CURRENT_USER
	| SESSION_USER
	| SYSTEM_USER
	| USER
	| CURRENT_CATALOG
	| CURRENT_SCHEMA
	| CAST '(' /*24L*/ a_expr AS Typename ')' /*24L*/
	| EXTRACT '(' /*24L*/ extract_list ')' /*24L*/
	| NORMALIZE '(' /*24L*/ a_expr ')' /*24L*/
	| NORMALIZE '(' /*24L*/ a_expr ',' unicode_normal_form ')' /*24L*/
	| OVERLAY '(' /*24L*/ overlay_list ')' /*24L*/
	| OVERLAY '(' /*24L*/ func_arg_list_opt ')' /*24L*/
	| POSITION '(' /*24L*/ position_list ')' /*24L*/
	| SUBSTRING '(' /*24L*/ substr_list ')' /*24L*/
	| SUBSTRING '(' /*24L*/ func_arg_list_opt ')' /*24L*/
	| TREAT '(' /*24L*/ a_expr AS Typename ')' /*24L*/
	| TRIM '(' /*24L*/ BOTH trim_list ')' /*24L*/
	| TRIM '(' /*24L*/ LEADING trim_list ')' /*24L*/
	| TRIM '(' /*24L*/ TRAILING trim_list ')' /*24L*/
	| TRIM '(' /*24L*/ trim_list ')' /*24L*/
	| NULLIF '(' /*24L*/ a_expr ',' a_expr ')' /*24L*/
	| COALESCE '(' /*24L*/ expr_list ')' /*24L*/
	| GREATEST '(' /*24L*/ expr_list ')' /*24L*/
	| LEAST '(' /*24L*/ expr_list ')' /*24L*/
	| XMLCONCAT '(' /*24L*/ expr_list ')' /*24L*/
	| XMLELEMENT '(' /*24L*/ NAME_P ColLabel ')' /*24L*/
	| XMLELEMENT '(' /*24L*/ NAME_P ColLabel ',' xml_attributes ')' /*24L*/
	| XMLELEMENT '(' /*24L*/ NAME_P ColLabel ',' expr_list ')' /*24L*/
	| XMLELEMENT '(' /*24L*/ NAME_P ColLabel ',' xml_attributes ',' expr_list ')' /*24L*/
	| XMLEXISTS '(' /*24L*/ c_expr xmlexists_argument ')' /*24L*/
	| XMLFOREST '(' /*24L*/ xml_attribute_list ')' /*24L*/
	| XMLPARSE '(' /*24L*/ document_or_content a_expr xml_whitespace_option ')' /*24L*/
	| XMLPI '(' /*24L*/ NAME_P ColLabel ')' /*24L*/
	| XMLPI '(' /*24L*/ NAME_P ColLabel ',' a_expr ')' /*24L*/
	| XMLROOT '(' /*24L*/ a_expr ',' xml_root_version opt_xml_root_standalone ')' /*24L*/
	| XMLSERIALIZE '(' /*24L*/ document_or_content a_expr AS SimpleTypename xml_indent_option ')' /*24L*/
	| JSON_OBJECT '(' /*24L*/ func_arg_list ')' /*24L*/
	| JSON_OBJECT '(' /*24L*/ json_name_and_value_list json_object_constructor_null_clause_opt json_key_uniqueness_constraint_opt json_output_clause_opt ')' /*24L*/
	| JSON_OBJECT '(' /*24L*/ json_output_clause_opt ')' /*24L*/
	| JSON_ARRAY '(' /*24L*/ json_value_expr_list json_array_constructor_null_clause_opt json_output_clause_opt ')' /*24L*/
	| JSON_ARRAY '(' /*24L*/ select_no_parens json_format_clause_opt json_output_clause_opt ')' /*24L*/
	| JSON_ARRAY '(' /*24L*/ json_output_clause_opt ')' /*24L*/
	;

xml_root_version :
	VERSION_P a_expr
	| VERSION_P NO VALUE_P /*12N*/
	;

opt_xml_root_standalone :
	',' STANDALONE_P YES_P
	| ',' STANDALONE_P NO
	| ',' STANDALONE_P NO VALUE_P /*12N*/
	| /*empty*/
	;

xml_attributes :
	XMLATTRIBUTES '(' /*24L*/ xml_attribute_list ')' /*24L*/
	;

xml_attribute_list :
	xml_attribute_el
	| xml_attribute_list ',' xml_attribute_el
	;

xml_attribute_el :
	a_expr AS ColLabel
	| a_expr
	;

document_or_content :
	DOCUMENT_P
	| CONTENT_P
	;

xml_indent_option :
	INDENT
	| NO INDENT
	| /*empty*/
	;

xml_whitespace_option :
	PRESERVE WHITESPACE_P
	| STRIP_P WHITESPACE_P
	| /*empty*/
	;

xmlexists_argument :
	PASSING c_expr
	| PASSING c_expr xml_passing_mech
	| PASSING xml_passing_mech c_expr
	| PASSING xml_passing_mech c_expr xml_passing_mech
	;

xml_passing_mech :
	BY REF_P
	| BY VALUE_P /*12N*/
	;

within_group_clause :
	WITHIN GROUP_P '(' /*24L*/ sort_clause ')' /*24L*/
	| /*empty*/
	;

filter_clause :
	FILTER '(' /*24L*/ WHERE a_expr ')' /*24L*/
	| /*empty*/
	;

window_clause :
	WINDOW window_definition_list
	| /*empty*/
	;

window_definition_list :
	window_definition
	| window_definition_list ',' window_definition
	;

window_definition :
	ColId AS window_specification
	;

over_clause :
	OVER window_specification
	| OVER ColId
	| /*empty*/
	;

window_specification :
	'(' /*24L*/ opt_existing_window_name opt_partition_clause opt_sort_clause opt_frame_clause ')' /*24L*/
	;

opt_existing_window_name :
	ColId
	| %prec Op /*16L*/ /*empty*/
	;

opt_partition_clause :
	PARTITION /*15N*/ BY expr_list
	| /*empty*/
	;

opt_frame_clause :
	RANGE /*15N*/ frame_extent opt_window_exclusion_clause
	| ROWS /*15N*/ frame_extent opt_window_exclusion_clause
	| GROUPS /*15N*/ frame_extent opt_window_exclusion_clause
	| /*empty*/
	;

frame_extent :
	frame_bound
	| BETWEEN /*9N*/ frame_bound AND /*5L*/ frame_bound
	;

frame_bound :
	UNBOUNDED /*14N*/ PRECEDING /*15N*/
	| UNBOUNDED /*14N*/ FOLLOWING /*15N*/
	| CURRENT_P ROW
	| a_expr PRECEDING /*15N*/
	| a_expr FOLLOWING /*15N*/
	;

opt_window_exclusion_clause :
	EXCLUDE CURRENT_P ROW
	| EXCLUDE GROUP_P
	| EXCLUDE TIES
	| EXCLUDE NO OTHERS
	| /*empty*/
	;

row :
	ROW '(' /*24L*/ expr_list ')' /*24L*/
	| ROW '(' /*24L*/ ')' /*24L*/
	| '(' /*24L*/ expr_list ',' a_expr ')' /*24L*/
	;

explicit_row :
	ROW '(' /*24L*/ expr_list ')' /*24L*/
	| ROW '(' /*24L*/ ')' /*24L*/
	;

implicit_row :
	'(' /*24L*/ expr_list ',' a_expr ')' /*24L*/
	;

sub_type :
	ANY
	| SOME
	| ALL
	;

all_Op :
	Op /*16L*/
	| MathOp
	;

MathOp :
	'+' /*17L*/
	| '-' /*17L*/
	| '*' /*18L*/
	| '/' /*18L*/
	| '%' /*18L*/
	| '^' /*19L*/
	| '<' /*8N*/
	| '>' /*8N*/
	| '=' /*8N*/
	| LESS_EQUALS /*8N*/
	| GREATER_EQUALS /*8N*/
	| NOT_EQUALS /*8N*/
	;

qual_Op :
	Op /*16L*/
	| OPERATOR /*16L*/ '(' /*24L*/ any_operator ')' /*24L*/
	;

qual_all_Op :
	all_Op
	| OPERATOR /*16L*/ '(' /*24L*/ any_operator ')' /*24L*/
	;

subquery_Op :
	all_Op
	| OPERATOR /*16L*/ '(' /*24L*/ any_operator ')' /*24L*/
	| LIKE /*9N*/
	| NOT_LA /*9N*/ LIKE /*9N*/
	| ILIKE /*9N*/
	| NOT_LA /*9N*/ ILIKE /*9N*/
	;

expr_list :
	a_expr
	| expr_list ',' a_expr
	;

func_arg_list :
	func_arg_expr
	| func_arg_list ',' func_arg_expr
	;

func_arg_expr :
	a_expr
	| param_name COLON_EQUALS a_expr
	| param_name EQUALS_GREATER a_expr
	;

func_arg_list_opt :
	func_arg_list
	| /*empty*/
	;

type_list :
	Typename
	| type_list ',' Typename
	;

array_expr :
	'[' /*23L*/ expr_list ']' /*23L*/
	| '[' /*23L*/ array_expr_list ']' /*23L*/
	| '[' /*23L*/ ']' /*23L*/
	;

array_expr_list :
	array_expr
	| array_expr_list ',' array_expr
	;

extract_list :
	extract_arg FROM a_expr
	;

extract_arg :
	IDENT /*15N*/
	| YEAR_P
	| MONTH_P
	| DAY_P
	| HOUR_P
	| MINUTE_P
	| SECOND_P
	| Sconst
	;

unicode_normal_form :
	NFC
	| NFD
	| NFKC
	| NFKD
	;

overlay_list :
	a_expr PLACING a_expr FROM a_expr FOR a_expr
	| a_expr PLACING a_expr FROM a_expr
	;

position_list :
	b_expr IN_P /*9N*/ b_expr
	;

substr_list :
	a_expr FROM a_expr FOR a_expr
	| a_expr FOR a_expr FROM a_expr
	| a_expr FROM a_expr
	| a_expr FOR a_expr
	| a_expr SIMILAR /*9N*/ a_expr ESCAPE /*10N*/ a_expr
	;

trim_list :
	a_expr FROM expr_list
	| FROM expr_list
	| expr_list
	;

in_expr :
	select_with_parens
	| '(' /*24L*/ expr_list ')' /*24L*/
	;

case_expr :
	CASE case_arg when_clause_list case_default END_P
	;

when_clause_list :
	when_clause
	| when_clause_list when_clause
	;

when_clause :
	WHEN a_expr THEN a_expr
	;

case_default :
	ELSE a_expr
	| /*empty*/
	;

case_arg :
	a_expr
	| /*empty*/
	;

columnref :
	ColId
	| ColId indirection
	;

indirection_el :
	'.' /*26L*/ attr_name
	| '.' /*26L*/ '*' /*18L*/
	| '[' /*23L*/ a_expr ']' /*23L*/
	| '[' /*23L*/ opt_slice_bound ':' opt_slice_bound ']' /*23L*/
	;

opt_slice_bound :
	a_expr
	| /*empty*/
	;

indirection :
	indirection_el
	| indirection indirection_el
	;

opt_indirection :
	/*empty*/
	| opt_indirection indirection_el
	;

opt_asymmetric :
	ASYMMETRIC
	| /*empty*/
	;

json_value_expr :
	a_expr json_format_clause_opt
	;

json_format_clause_opt :
	FORMAT_LA JSON /*11N*/ json_encoding_clause_opt
	| /*empty*/
	;

json_encoding_clause_opt :
	ENCODING name
	| /*empty*/
	;

json_output_clause_opt :
	RETURNING Typename json_format_clause_opt
	| /*empty*/
	;

json_predicate_type_constraint :
	JSON /*11N*/
	| JSON /*11N*/ VALUE_P /*12N*/
	| JSON /*11N*/ ARRAY
	| JSON /*11N*/ OBJECT_P /*12N*/
	| JSON /*11N*/ SCALAR /*12N*/
	;

json_key_uniqueness_constraint_opt :
	WITH /*13N*/ UNIQUE /*11N*/ KEYS /*12N*/
	| WITH /*13N*/ UNIQUE /*11N*/
	| WITHOUT /*13N*/ UNIQUE /*11N*/ KEYS /*12N*/
	| WITHOUT /*13N*/ UNIQUE /*11N*/
	| %prec KEYS /*12N*/ /*empty*/
	;

json_name_and_value_list :
	json_name_and_value
	| json_name_and_value_list ',' json_name_and_value
	;

json_name_and_value :
	c_expr VALUE_P /*12N*/ json_value_expr
	| a_expr ':' json_value_expr
	;

json_object_constructor_null_clause_opt :
	NULL_P ON NULL_P
	| ABSENT ON NULL_P
	| /*empty*/
	;

json_array_constructor_null_clause_opt :
	NULL_P ON NULL_P
	| ABSENT ON NULL_P
	| /*empty*/
	;

json_value_expr_list :
	json_value_expr
	| json_value_expr_list ',' json_value_expr
	;

json_aggregate_func :
	JSON_OBJECTAGG '(' /*24L*/ json_name_and_value json_object_constructor_null_clause_opt json_key_uniqueness_constraint_opt json_output_clause_opt ')' /*24L*/
	| JSON_ARRAYAGG '(' /*24L*/ json_value_expr json_array_aggregate_order_by_clause_opt json_array_constructor_null_clause_opt json_output_clause_opt ')' /*24L*/
	;

json_array_aggregate_order_by_clause_opt :
	ORDER BY sortby_list
	| /*empty*/
	;

opt_target_list :
	target_list
	| /*empty*/
	;

target_list :
	target_el
	| target_list ',' target_el
	;

target_el :
	a_expr AS ColLabel
	| a_expr BareColLabel
	| a_expr
	| '*' /*18L*/
	;

qualified_name_list :
	qualified_name
	| qualified_name_list ',' qualified_name
	;

qualified_name :
	ColId
	| ColId indirection
	;

name_list :
	name
	| name_list ',' name
	;

name :
	ColId
	;

attr_name :
	ColLabel
	;

file_name :
	Sconst
	;

func_name :
	type_function_name
	| ColId indirection
	;

AexprConst :
	Iconst
	| FCONST
	| Sconst
	| BCONST
	| XCONST
	| func_name Sconst
	| func_name '(' /*24L*/ func_arg_list opt_sort_clause ')' /*24L*/ Sconst
	| ConstTypename Sconst
	| ConstInterval Sconst opt_interval
	| ConstInterval '(' /*24L*/ Iconst ')' /*24L*/ Sconst
	| TRUE_P
	| FALSE_P
	| NULL_P
	;

Iconst :
	ICONST
	;

Sconst :
	SCONST
	;

SignedIconst :
	Iconst
	| '+' /*17L*/ Iconst
	| '-' /*17L*/ Iconst
	;

RoleId :
	RoleSpec
	;

RoleSpec :
	NonReservedWord
	| CURRENT_ROLE
	| CURRENT_USER
	| SESSION_USER
	;

role_list :
	RoleSpec
	| role_list ',' RoleSpec
	;

PLpgSQL_Expr :
	opt_distinct_clause opt_target_list from_clause where_clause group_clause having_clause window_clause opt_sort_clause opt_select_limit opt_for_locking_clause
	;

PLAssignStmt :
	plassign_target opt_indirection plassign_equals PLpgSQL_Expr
	;

plassign_target :
	ColId
	| PARAM
	;

plassign_equals :
	COLON_EQUALS
	| '=' /*8N*/
	;

ColId :
	IDENT /*15N*/
	| unreserved_keyword
	| col_name_keyword
	;

type_function_name :
	IDENT /*15N*/
	| unreserved_keyword
	| type_func_name_keyword
	;

NonReservedWord :
	IDENT /*15N*/
	| unreserved_keyword
	| col_name_keyword
	| type_func_name_keyword
	;

ColLabel :
	IDENT /*15N*/
	| unreserved_keyword
	| col_name_keyword
	| type_func_name_keyword
	| reserved_keyword
	;

BareColLabel :
	IDENT /*15N*/
	| bare_label_keyword
	;

unreserved_keyword :
	ABORT_P
	| ABSENT
	| ABSOLUTE_P
	| ACCESS
	| ACTION
	| ADD_P
	| ADMIN
	| AFTER
	| AGGREGATE
	| ALSO
	| ALTER
	| ALWAYS
	| ASENSITIVE
	| ASSERTION
	| ASSIGNMENT
	| AT /*20L*/
	| ATOMIC
	| ATTACH
	| ATTRIBUTE
	| BACKWARD
	| BEFORE
	| BEGIN_P
	| BREADTH
	| BY
	| CACHE
	| CALL
	| CALLED
	| CASCADE
	| CASCADED
	| CATALOG_P
	| CHAIN
	| CHARACTERISTICS
	| CHECKPOINT
	| CLASS
	| CLOSE
	| CLUSTER
	| COLUMNS
	| COMMENT
	| COMMENTS
	| COMMIT
	| COMMITTED
	| COMPRESSION
	| CONFIGURATION
	| CONFLICT
	| CONNECTION
	| CONSTRAINTS
	| CONTENT_P
	| CONTINUE_P
	| CONVERSION_P
	| COPY
	| COST
	| CSV
	| CUBE /*15N*/
	| CURRENT_P
	| CURSOR
	| CYCLE
	| DATA_P
	| DATABASE
	| DAY_P
	| DEALLOCATE
	| DECLARE
	| DEFAULTS
	| DEFERRED
	| DEFINER
	| DELETE_P
	| DELIMITER
	| DELIMITERS
	| DEPENDS
	| DEPTH
	| DETACH
	| DICTIONARY
	| DISABLE_P
	| DISCARD
	| DOCUMENT_P
	| DOMAIN_P
	| DOUBLE_P
	| DROP
	| EACH
	| ENABLE_P
	| ENCODING
	| ENCRYPTED
	| ENUM_P
	| ESCAPE /*10N*/
	| EVENT
	| EXCLUDE
	| EXCLUDING
	| EXCLUSIVE
	| EXECUTE
	| EXPLAIN
	| EXPRESSION
	| EXTENSION
	| EXTERNAL
	| FAMILY
	| FILTER
	| FINALIZE
	| FIRST_P
	| FOLLOWING /*15N*/
	| FORCE
	| FORMAT
	| FORWARD
	| FUNCTION
	| FUNCTIONS
	| GENERATED
	| GLOBAL
	| GRANTED
	| GROUPS /*15N*/
	| HANDLER
	| HEADER_P
	| HOLD
	| HOUR_P
	| IDENTITY_P
	| IF_P
	| IMMEDIATE
	| IMMUTABLE
	| IMPLICIT_P
	| IMPORT_P
	| INCLUDE
	| INCLUDING
	| INCREMENT
	| INDENT
	| INDEX
	| INDEXES
	| INHERIT
	| INHERITS
	| INLINE_P
	| INPUT_P
	| INSENSITIVE
	| INSERT
	| INSTEAD
	| INVOKER
	| ISOLATION
	| JSON /*11N*/
	| KEY
	| KEYS /*12N*/
	| LABEL
	| LANGUAGE
	| LARGE_P
	| LAST_P
	| LEAKPROOF
	| LEVEL
	| LISTEN
	| LOAD
	| LOCAL
	| LOCATION
	| LOCK_P
	| LOCKED
	| LOGGED
	| MAPPING
	| MATCH
	| MATCHED
	| MATERIALIZED
	| MAXVALUE
	| MERGE
	| METHOD
	| MINUTE_P
	| MINVALUE
	| MODE
	| MONTH_P
	| MOVE
	| NAME_P
	| NAMES
	| NEW
	| NEXT
	| NFC
	| NFD
	| NFKC
	| NFKD
	| NO
	| NORMALIZED
	| NOTHING
	| NOTIFY
	| NOWAIT
	| NULLS_P
	| OBJECT_P /*12N*/
	| OF
	| OFF
	| OIDS
	| OLD
	| OPERATOR /*16L*/
	| OPTION
	| OPTIONS
	| ORDINALITY
	| OTHERS
	| OVER
	| OVERRIDING
	| OWNED
	| OWNER
	| PARALLEL
	| PARAMETER
	| PARSER
	| PARTIAL
	| PARTITION /*15N*/
	| PASSING
	| PASSWORD
	| PLANS
	| POLICY
	| PRECEDING /*15N*/
	| PREPARE
	| PREPARED
	| PRESERVE
	| PRIOR
	| PRIVILEGES
	| PROCEDURAL
	| PROCEDURE
	| PROCEDURES
	| PROGRAM
	| PUBLICATION
	| QUOTE
	| RANGE /*15N*/
	| READ
	| REASSIGN
	| RECHECK
	| RECURSIVE
	| REF_P
	| REFERENCING
	| REFRESH
	| REINDEX
	| RELATIVE_P
	| RELEASE
	| RENAME
	| REPEATABLE
	| REPLACE
	| REPLICA
	| RESET
	| RESTART
	| RESTRICT
	| RETURN
	| RETURNS
	| REVOKE
	| ROLE
	| ROLLBACK
	| ROLLUP /*15N*/
	| ROUTINE
	| ROUTINES
	| ROWS /*15N*/
	| RULE
	| SAVEPOINT
	| SCALAR /*12N*/
	| SCHEMA
	| SCHEMAS
	| SCROLL
	| SEARCH
	| SECOND_P
	| SECURITY
	| SEQUENCE
	| SEQUENCES
	| SERIALIZABLE
	| SERVER
	| SESSION
	| SET /*1N*/
	| SETS
	| SHARE
	| SHOW
	| SIMPLE
	| SKIP
	| SNAPSHOT
	| SQL_P
	| STABLE
	| STANDALONE_P
	| START
	| STATEMENT
	| STATISTICS
	| STDIN
	| STDOUT
	| STORAGE
	| STORED
	| STRICT_P
	| STRIP_P
	| SUBSCRIPTION
	| SUPPORT
	| SYSID
	| SYSTEM_P
	| TABLES
	| TABLESPACE
	| TEMP
	| TEMPLATE
	| TEMPORARY
	| TEXT_P
	| TIES
	| TRANSACTION
	| TRANSFORM
	| TRIGGER
	| TRUNCATE
	| TRUSTED
	| TYPE_P
	| TYPES_P
	| UESCAPE
	| UNBOUNDED /*14N*/
	| UNCOMMITTED
	| UNENCRYPTED
	| UNKNOWN
	| UNLISTEN
	| UNLOGGED
	| UNTIL
	| UPDATE
	| VACUUM
	| VALID
	| VALIDATE
	| VALIDATOR
	| VALUE_P /*12N*/
	| VARYING
	| VERSION_P
	| VIEW
	| VIEWS
	| VOLATILE
	| WHITESPACE_P
	| WITHIN
	| WITHOUT /*13N*/
	| WORK
	| WRAPPER
	| WRITE
	| XML_P
	| YEAR_P
	| YES_P
	| ZONE
	;

col_name_keyword :
	BETWEEN /*9N*/
	| BIGINT
	| BIT
	| BOOLEAN_P
	| CHAR_P
	| CHARACTER
	| COALESCE
	| DEC
	| DECIMAL_P
	| EXISTS
	| EXTRACT
	| FLOAT_P
	| GREATEST
	| GROUPING
	| INOUT
	| INT_P
	| INTEGER
	| INTERVAL
	| JSON_ARRAY
	| JSON_ARRAYAGG
	| JSON_OBJECT
	| JSON_OBJECTAGG
	| LEAST
	| NATIONAL
	| NCHAR
	| NONE
	| NORMALIZE
	| NULLIF
	| NUMERIC
	| OUT_P
	| OVERLAY
	| POSITION
	| PRECISION
	| REAL
	| ROW
	| SETOF
	| SMALLINT
	| SUBSTRING
	| TIME
	| TIMESTAMP
	| TREAT
	| TRIM
	| VALUES
	| VARCHAR
	| XMLATTRIBUTES
	| XMLCONCAT
	| XMLELEMENT
	| XMLEXISTS
	| XMLFOREST
	| XMLNAMESPACES
	| XMLPARSE
	| XMLPI
	| XMLROOT
	| XMLSERIALIZE
	| XMLTABLE
	;

type_func_name_keyword :
	AUTHORIZATION
	| BINARY
	| COLLATION
	| CONCURRENTLY
	| CROSS /*27L*/
	| CURRENT_SCHEMA
	| FREEZE
	| FULL /*27L*/
	| ILIKE /*9N*/
	| INNER_P /*27L*/
	| IS /*7N*/
	| ISNULL /*7N*/
	| JOIN /*27L*/
	| LEFT /*27L*/
	| LIKE /*9N*/
	| NATURAL /*27L*/
	| NOTNULL /*7N*/
	| OUTER_P
	| OVERLAPS
	| RIGHT /*27L*/
	| SIMILAR /*9N*/
	| TABLESAMPLE
	| VERBOSE
	;

reserved_keyword :
	ALL
	| ANALYSE
	| ANALYZE
	| AND /*5L*/
	| ANY
	| ARRAY
	| AS
	| ASC
	| ASYMMETRIC
	| BOTH
	| CASE
	| CAST
	| CHECK
	| COLLATE /*21L*/
	| COLUMN
	| CONSTRAINT
	| CREATE
	| CURRENT_CATALOG
	| CURRENT_DATE
	| CURRENT_ROLE
	| CURRENT_TIME
	| CURRENT_TIMESTAMP
	| CURRENT_USER
	| DEFAULT
	| DEFERRABLE
	| DESC
	| DISTINCT
	| DO
	| ELSE
	| END_P
	| EXCEPT /*2L*/
	| FALSE_P
	| FETCH
	| FOR
	| FOREIGN
	| FROM
	| GRANT
	| GROUP_P
	| HAVING
	| IN_P /*9N*/
	| INITIALLY
	| INTERSECT /*3L*/
	| INTO
	| LATERAL_P
	| LEADING
	| LIMIT
	| LOCALTIME
	| LOCALTIMESTAMP
	| NOT /*6R*/
	| NULL_P
	| OFFSET
	| ON
	| ONLY
	| OR /*4L*/
	| ORDER
	| PLACING
	| PRIMARY
	| REFERENCES
	| RETURNING
	| SELECT
	| SESSION_USER
	| SOME
	| SYMMETRIC
	| SYSTEM_USER
	| TABLE
	| THEN
	| TO
	| TRAILING
	| TRUE_P
	| UNION /*2L*/
	| UNIQUE /*11N*/
	| USER
	| USING
	| VARIADIC
	| WHEN
	| WHERE
	| WINDOW
	| WITH /*13N*/
	;

bare_label_keyword :
	ABORT_P
	| ABSENT
	| ABSOLUTE_P
	| ACCESS
	| ACTION
	| ADD_P
	| ADMIN
	| AFTER
	| AGGREGATE
	| ALL
	| ALSO
	| ALTER
	| ALWAYS
	| ANALYSE
	| ANALYZE
	| AND /*5L*/
	| ANY
	| ASC
	| ASENSITIVE
	| ASSERTION
	| ASSIGNMENT
	| ASYMMETRIC
	| AT /*20L*/
	| ATOMIC
	| ATTACH
	| ATTRIBUTE
	| AUTHORIZATION
	| BACKWARD
	| BEFORE
	| BEGIN_P
	| BETWEEN /*9N*/
	| BIGINT
	| BINARY
	| BIT
	| BOOLEAN_P
	| BOTH
	| BREADTH
	| BY
	| CACHE
	| CALL
	| CALLED
	| CASCADE
	| CASCADED
	| CASE
	| CAST
	| CATALOG_P
	| CHAIN
	| CHARACTERISTICS
	| CHECK
	| CHECKPOINT
	| CLASS
	| CLOSE
	| CLUSTER
	| COALESCE
	| COLLATE /*21L*/
	| COLLATION
	| COLUMN
	| COLUMNS
	| COMMENT
	| COMMENTS
	| COMMIT
	| COMMITTED
	| COMPRESSION
	| CONCURRENTLY
	| CONFIGURATION
	| CONFLICT
	| CONNECTION
	| CONSTRAINT
	| CONSTRAINTS
	| CONTENT_P
	| CONTINUE_P
	| CONVERSION_P
	| COPY
	| COST
	| CROSS /*27L*/
	| CSV
	| CUBE /*15N*/
	| CURRENT_P
	| CURRENT_CATALOG
	| CURRENT_DATE
	| CURRENT_ROLE
	| CURRENT_SCHEMA
	| CURRENT_TIME
	| CURRENT_TIMESTAMP
	| CURRENT_USER
	| CURSOR
	| CYCLE
	| DATA_P
	| DATABASE
	| DEALLOCATE
	| DEC
	| DECIMAL_P
	| DECLARE
	| DEFAULT
	| DEFAULTS
	| DEFERRABLE
	| DEFERRED
	| DEFINER
	| DELETE_P
	| DELIMITER
	| DELIMITERS
	| DEPENDS
	| DEPTH
	| DESC
	| DETACH
	| DICTIONARY
	| DISABLE_P
	| DISCARD
	| DISTINCT
	| DO
	| DOCUMENT_P
	| DOMAIN_P
	| DOUBLE_P
	| DROP
	| EACH
	| ELSE
	| ENABLE_P
	| ENCODING
	| ENCRYPTED
	| END_P
	| ENUM_P
	| ESCAPE /*10N*/
	| EVENT
	| EXCLUDE
	| EXCLUDING
	| EXCLUSIVE
	| EXECUTE
	| EXISTS
	| EXPLAIN
	| EXPRESSION
	| EXTENSION
	| EXTERNAL
	| EXTRACT
	| FALSE_P
	| FAMILY
	| FINALIZE
	| FIRST_P
	| FLOAT_P
	| FOLLOWING /*15N*/
	| FORCE
	| FOREIGN
	| FORMAT
	| FORWARD
	| FREEZE
	| FULL /*27L*/
	| FUNCTION
	| FUNCTIONS
	| GENERATED
	| GLOBAL
	| GRANTED
	| GREATEST
	| GROUPING
	| GROUPS /*15N*/
	| HANDLER
	| HEADER_P
	| HOLD
	| IDENTITY_P
	| IF_P
	| ILIKE /*9N*/
	| IMMEDIATE
	| IMMUTABLE
	| IMPLICIT_P
	| IMPORT_P
	| IN_P /*9N*/
	| INCLUDE
	| INCLUDING
	| INCREMENT
	| INDENT
	| INDEX
	| INDEXES
	| INHERIT
	| INHERITS
	| INITIALLY
	| INLINE_P
	| INNER_P /*27L*/
	| INOUT
	| INPUT_P
	| INSENSITIVE
	| INSERT
	| INSTEAD
	| INT_P
	| INTEGER
	| INTERVAL
	| INVOKER
	| IS /*7N*/
	| ISOLATION
	| JOIN /*27L*/
	| JSON /*11N*/
	| JSON_ARRAY
	| JSON_ARRAYAGG
	| JSON_OBJECT
	| JSON_OBJECTAGG
	| KEY
	| KEYS /*12N*/
	| LABEL
	| LANGUAGE
	| LARGE_P
	| LAST_P
	| LATERAL_P
	| LEADING
	| LEAKPROOF
	| LEAST
	| LEFT /*27L*/
	| LEVEL
	| LIKE /*9N*/
	| LISTEN
	| LOAD
	| LOCAL
	| LOCALTIME
	| LOCALTIMESTAMP
	| LOCATION
	| LOCK_P
	| LOCKED
	| LOGGED
	| MAPPING
	| MATCH
	| MATCHED
	| MATERIALIZED
	| MAXVALUE
	| MERGE
	| METHOD
	| MINVALUE
	| MODE
	| MOVE
	| NAME_P
	| NAMES
	| NATIONAL
	| NATURAL /*27L*/
	| NCHAR
	| NEW
	| NEXT
	| NFC
	| NFD
	| NFKC
	| NFKD
	| NO
	| NONE
	| NORMALIZE
	| NORMALIZED
	| NOT /*6R*/
	| NOTHING
	| NOTIFY
	| NOWAIT
	| NULL_P
	| NULLIF
	| NULLS_P
	| NUMERIC
	| OBJECT_P /*12N*/
	| OF
	| OFF
	| OIDS
	| OLD
	| ONLY
	| OPERATOR /*16L*/
	| OPTION
	| OPTIONS
	| OR /*4L*/
	| ORDINALITY
	| OTHERS
	| OUT_P
	| OUTER_P
	| OVERLAY
	| OVERRIDING
	| OWNED
	| OWNER
	| PARALLEL
	| PARAMETER
	| PARSER
	| PARTIAL
	| PARTITION /*15N*/
	| PASSING
	| PASSWORD
	| PLACING
	| PLANS
	| POLICY
	| POSITION
	| PRECEDING /*15N*/
	| PREPARE
	| PREPARED
	| PRESERVE
	| PRIMARY
	| PRIOR
	| PRIVILEGES
	| PROCEDURAL
	| PROCEDURE
	| PROCEDURES
	| PROGRAM
	| PUBLICATION
	| QUOTE
	| RANGE /*15N*/
	| READ
	| REAL
	| REASSIGN
	| RECHECK
	| RECURSIVE
	| REF_P
	| REFERENCES
	| REFERENCING
	| REFRESH
	| REINDEX
	| RELATIVE_P
	| RELEASE
	| RENAME
	| REPEATABLE
	| REPLACE
	| REPLICA
	| RESET
	| RESTART
	| RESTRICT
	| RETURN
	| RETURNS
	| REVOKE
	| RIGHT /*27L*/
	| ROLE
	| ROLLBACK
	| ROLLUP /*15N*/
	| ROUTINE
	| ROUTINES
	| ROW
	| ROWS /*15N*/
	| RULE
	| SAVEPOINT
	| SCALAR /*12N*/
	| SCHEMA
	| SCHEMAS
	| SCROLL
	| SEARCH
	| SECURITY
	| SELECT
	| SEQUENCE
	| SEQUENCES
	| SERIALIZABLE
	| SERVER
	| SESSION
	| SESSION_USER
	| SET /*1N*/
	| SETOF
	| SETS
	| SHARE
	| SHOW
	| SIMILAR /*9N*/
	| SIMPLE
	| SKIP
	| SMALLINT
	| SNAPSHOT
	| SOME
	| SQL_P
	| STABLE
	| STANDALONE_P
	| START
	| STATEMENT
	| STATISTICS
	| STDIN
	| STDOUT
	| STORAGE
	| STORED
	| STRICT_P
	| STRIP_P
	| SUBSCRIPTION
	| SUBSTRING
	| SUPPORT
	| SYMMETRIC
	| SYSID
	| SYSTEM_P
	| SYSTEM_USER
	| TABLE
	| TABLES
	| TABLESAMPLE
	| TABLESPACE
	| TEMP
	| TEMPLATE
	| TEMPORARY
	| TEXT_P
	| THEN
	| TIES
	| TIME
	| TIMESTAMP
	| TRAILING
	| TRANSACTION
	| TRANSFORM
	| TREAT
	| TRIGGER
	| TRIM
	| TRUE_P
	| TRUNCATE
	| TRUSTED
	| TYPE_P
	| TYPES_P
	| UESCAPE
	| UNBOUNDED /*14N*/
	| UNCOMMITTED
	| UNENCRYPTED
	| UNIQUE /*11N*/
	| UNKNOWN
	| UNLISTEN
	| UNLOGGED
	| UNTIL
	| UPDATE
	| USER
	| USING
	| VACUUM
	| VALID
	| VALIDATE
	| VALIDATOR
	| VALUE_P /*12N*/
	| VALUES
	| VARCHAR
	| VARIADIC
	| VERBOSE
	| VERSION_P
	| VIEW
	| VIEWS
	| VOLATILE
	| WHEN
	| WHITESPACE_P
	| WORK
	| WRAPPER
	| WRITE
	| XML_P
	| XMLATTRIBUTES
	| XMLCONCAT
	| XMLELEMENT
	| XMLEXISTS
	| XMLFOREST
	| XMLNAMESPACES
	| XMLPARSE
	| XMLPI
	| XMLROOT
	| XMLSERIALIZE
	| XMLTABLE
	| YES_P
	| ZONE
	;

// Tokens

/*
 * List of keyword (name, token-value, category, bare-label-status) entries.
 *
 * Note: gen_keywordlist.pl requires the entries to appear in ASCII order.
 */

 //PG_KEYWORD("\([^"]+\)", \(\w[^,]+\),.+
/* name, value, category, is-bare-label */
ABORT_P : 'ABORT' ;
ABSENT : 'ABSENT' ;
ABSOLUTE_P : 'ABSOLUTE' ;
ACCESS : 'ACCESS' ;
ACTION : 'ACTION' ;
ADD_P : 'ADD' ;
ADMIN : 'ADMIN' ;
AFTER : 'AFTER' ;
AGGREGATE : 'AGGREGATE' ;
ALL : 'ALL' ;
ALSO : 'ALSO' ;
ALTER : 'ALTER' ;
ALWAYS : 'ALWAYS' ;
ANALYSE : 'ANALYSE' ;
ANALYZE : 'ANALYZE' ;
AND : 'AND' ;
ANY : 'ANY' ;
ARRAY : 'ARRAY' ;
AS : 'AS' ;
ASC : 'ASC' ;
ASENSITIVE : 'ASENSITIVE' ;
ASSERTION : 'ASSERTION' ;
ASSIGNMENT : 'ASSIGNMENT' ;
ASYMMETRIC : 'ASYMMETRIC' ;
AT : 'AT' ;
ATOMIC : 'ATOMIC' ;
ATTACH : 'ATTACH' ;
ATTRIBUTE : 'ATTRIBUTE' ;
AUTHORIZATION : 'AUTHORIZATION' ;
BACKWARD : 'BACKWARD' ;
BEFORE : 'BEFORE' ;
BEGIN_P : 'BEGIN' ;
BETWEEN : 'BETWEEN' ;
BIGINT : 'BIGINT' ;
BINARY : 'BINARY' ;
BIT : 'BIT' ;
BOOLEAN_P : 'BOOLEAN' ;
BOTH : 'BOTH' ;
BREADTH : 'BREADTH' ;
BY : 'BY' ;
CACHE : 'CACHE' ;
CALL : 'CALL' ;
CALLED : 'CALLED' ;
CASCADE : 'CASCADE' ;
CASCADED : 'CASCADED' ;
CASE : 'CASE' ;
CAST : 'CAST' ;
CATALOG_P : 'CATALOG' ;
CHAIN : 'CHAIN' ;
CHAR_P : 'CHAR' ;
CHARACTER : 'CHARACTER' ;
CHARACTERISTICS : 'CHARACTERISTICS' ;
CHECK : 'CHECK' ;
CHECKPOINT : 'CHECKPOINT' ;
CLASS : 'CLASS' ;
CLOSE : 'CLOSE' ;
CLUSTER : 'CLUSTER' ;
COALESCE : 'COALESCE' ;
COLLATE : 'COLLATE' ;
COLLATION : 'COLLATION' ;
COLUMN : 'COLUMN' ;
COLUMNS : 'COLUMNS' ;
COMMENT : 'COMMENT' ;
COMMENTS : 'COMMENTS' ;
COMMIT : 'COMMIT' ;
COMMITTED : 'COMMITTED' ;
COMPRESSION : 'COMPRESSION' ;
CONCURRENTLY : 'CONCURRENTLY' ;
CONFIGURATION : 'CONFIGURATION' ;
CONFLICT : 'CONFLICT' ;
CONNECTION : 'CONNECTION' ;
CONSTRAINT : 'CONSTRAINT' ;
CONSTRAINTS : 'CONSTRAINTS' ;
CONTENT_P : 'CONTENT' ;
CONTINUE_P : 'CONTINUE' ;
CONVERSION_P : 'CONVERSION' ;
COPY : 'COPY' ;
COST : 'COST' ;
CREATE : 'CREATE' ;
CROSS : 'CROSS' ;
CSV : 'CSV' ;
CUBE : 'CUBE' ;
CURRENT_P : 'CURRENT' ;
CURRENT_CATALOG : 'CURRENT_CATALOG' ;
CURRENT_DATE : 'CURRENT_DATE' ;
CURRENT_ROLE : 'CURRENT_ROLE' ;
CURRENT_SCHEMA : 'CURRENT_SCHEMA' ;
CURRENT_TIME : 'CURRENT_TIME' ;
CURRENT_TIMESTAMP : 'CURRENT_TIMESTAMP' ;
CURRENT_USER : 'CURRENT_USER' ;
CURSOR : 'CURSOR' ;
CYCLE : 'CYCLE' ;
DATA_P : 'DATA' ;
DATABASE : 'DATABASE' ;
DAY_P : 'DAY' ;
DEALLOCATE : 'DEALLOCATE' ;
DEC : 'DEC' ;
DECIMAL_P : 'DECIMAL' ;
DECLARE : 'DECLARE' ;
DEFAULT : 'DEFAULT' ;
DEFAULTS : 'DEFAULTS' ;
DEFERRABLE : 'DEFERRABLE' ;
DEFERRED : 'DEFERRED' ;
DEFINER : 'DEFINER' ;
DELETE_P : 'DELETE' ;
DELIMITER : 'DELIMITER' ;
DELIMITERS : 'DELIMITERS' ;
DEPENDS : 'DEPENDS' ;
DEPTH : 'DEPTH' ;
DESC : 'DESC' ;
DETACH : 'DETACH' ;
DICTIONARY : 'DICTIONARY' ;
DISABLE_P : 'DISABLE' ;
DISCARD : 'DISCARD' ;
DISTINCT : 'DISTINCT' ;
DO : 'DO' ;
DOCUMENT_P : 'DOCUMENT' ;
DOMAIN_P : 'DOMAIN' ;
DOUBLE_P : 'DOUBLE' ;
DROP : 'DROP' ;
EACH : 'EACH' ;
ELSE : 'ELSE' ;
ENABLE_P : 'ENABLE' ;
ENCODING : 'ENCODING' ;
ENCRYPTED : 'ENCRYPTED' ;
END_P : 'END' ;
ENUM_P : 'ENUM' ;
ESCAPE : 'ESCAPE' ;
EVENT : 'EVENT' ;
EXCEPT : 'EXCEPT' ;
EXCLUDE : 'EXCLUDE' ;
EXCLUDING : 'EXCLUDING' ;
EXCLUSIVE : 'EXCLUSIVE' ;
EXECUTE : 'EXECUTE' ;
EXISTS : 'EXISTS' ;
EXPLAIN : 'EXPLAIN' ;
EXPRESSION : 'EXPRESSION' ;
EXTENSION : 'EXTENSION' ;
EXTERNAL : 'EXTERNAL' ;
EXTRACT : 'EXTRACT' ;
FALSE_P : 'FALSE' ;
FAMILY : 'FAMILY' ;
FETCH : 'FETCH' ;
FILTER : 'FILTER' ;
FINALIZE : 'FINALIZE' ;
FIRST_P : 'FIRST' ;
FLOAT_P : 'FLOAT' ;
FOLLOWING : 'FOLLOWING' ;
FOR : 'FOR' ;
FORCE : 'FORCE' ;
FOREIGN : 'FOREIGN' ;
FORMAT : 'FORMAT' ;
FORWARD : 'FORWARD' ;
FREEZE : 'FREEZE' ;
FROM : 'FROM' ;
FULL : 'FULL' ;
FUNCTION : 'FUNCTION' ;
FUNCTIONS : 'FUNCTIONS' ;
GENERATED : 'GENERATED' ;
GLOBAL : 'GLOBAL' ;
GRANT : 'GRANT' ;
GRANTED : 'GRANTED' ;
GREATEST : 'GREATEST' ;
GROUP_P : 'GROUP' ;
GROUPING : 'GROUPING' ;
GROUPS : 'GROUPS' ;
HANDLER : 'HANDLER' ;
HAVING : 'HAVING' ;
HEADER_P : 'HEADER' ;
HOLD : 'HOLD' ;
HOUR_P : 'HOUR' ;
IDENTITY_P : 'IDENTITY' ;
IF_P : 'IF' ;
ILIKE : 'ILIKE' ;
IMMEDIATE : 'IMMEDIATE' ;
IMMUTABLE : 'IMMUTABLE' ;
IMPLICIT_P : 'IMPLICIT' ;
IMPORT_P : 'IMPORT' ;
IN_P : 'IN' ;
INCLUDE : 'INCLUDE' ;
INCLUDING : 'INCLUDING' ;
INCREMENT : 'INCREMENT' ;
INDENT : 'INDENT' ;
INDEX : 'INDEX' ;
INDEXES : 'INDEXES' ;
INHERIT : 'INHERIT' ;
INHERITS : 'INHERITS' ;
INITIALLY : 'INITIALLY' ;
INLINE_P : 'INLINE' ;
INNER_P : 'INNER' ;
INOUT : 'INOUT' ;
INPUT_P : 'INPUT' ;
INSENSITIVE : 'INSENSITIVE' ;
INSERT : 'INSERT' ;
INSTEAD : 'INSTEAD' ;
INT_P : 'INT' ;
INTEGER : 'INTEGER' ;
INTERSECT : 'INTERSECT' ;
INTERVAL : 'INTERVAL' ;
INTO : 'INTO' ;
INVOKER : 'INVOKER' ;
IS : 'IS' ;
ISNULL : 'ISNULL' ;
ISOLATION : 'ISOLATION' ;
JOIN : 'JOIN' ;
JSON : 'JSON' ;
JSON_ARRAY : 'JSON_ARRAY' ;
JSON_ARRAYAGG : 'JSON_ARRAYAGG' ;
JSON_OBJECT : 'JSON_OBJECT' ;
JSON_OBJECTAGG : 'JSON_OBJECTAGG' ;
KEY : 'KEY' ;
KEYS : 'KEYS' ;
LABEL : 'LABEL' ;
LANGUAGE : 'LANGUAGE' ;
LARGE_P : 'LARGE' ;
LAST_P : 'LAST' ;
LATERAL_P : 'LATERAL' ;
LEADING : 'LEADING' ;
LEAKPROOF : 'LEAKPROOF' ;
LEAST : 'LEAST' ;
LEFT : 'LEFT' ;
LEVEL : 'LEVEL' ;
LIKE : 'LIKE' ;
LIMIT : 'LIMIT' ;
LISTEN : 'LISTEN' ;
LOAD : 'LOAD' ;
LOCAL : 'LOCAL' ;
LOCALTIME : 'LOCALTIME' ;
LOCALTIMESTAMP : 'LOCALTIMESTAMP' ;
LOCATION : 'LOCATION' ;
LOCK_P : 'LOCK' ;
LOCKED : 'LOCKED' ;
LOGGED : 'LOGGED' ;
MAPPING : 'MAPPING' ;
MATCH : 'MATCH' ;
MATCHED : 'MATCHED' ;
MATERIALIZED : 'MATERIALIZED' ;
MAXVALUE : 'MAXVALUE' ;
MERGE : 'MERGE' ;
METHOD : 'METHOD' ;
MINUTE_P : 'MINUTE' ;
MINVALUE : 'MINVALUE' ;
MODE : 'MODE' ;
MONTH_P : 'MONTH' ;
MOVE : 'MOVE' ;
NAME_P : 'NAME' ;
NAMES : 'NAMES' ;
NATIONAL : 'NATIONAL' ;
NATURAL : 'NATURAL' ;
NCHAR : 'NCHAR' ;
NEW : 'NEW' ;
NEXT : 'NEXT' ;
NFC : 'NFC' ;
NFD : 'NFD' ;
NFKC : 'NFKC' ;
NFKD : 'NFKD' ;
NO : 'NO' ;
NONE : 'NONE' ;
NORMALIZE : 'NORMALIZE' ;
NORMALIZED : 'NORMALIZED' ;
NOT : 'NOT' ;
NOTHING : 'NOTHING' ;
NOTIFY : 'NOTIFY' ;
NOTNULL : 'NOTNULL' ;
NOWAIT : 'NOWAIT' ;
NULL_P : 'NULL' ;
NULLIF : 'NULLIF' ;
NULLS_P : 'NULLS' ;
NUMERIC : 'NUMERIC' ;
OBJECT_P : 'OBJECT' ;
OF : 'OF' ;
OFF : 'OFF' ;
OFFSET : 'OFFSET' ;
OIDS : 'OIDS' ;
OLD : 'OLD' ;
ON : 'ON' ;
ONLY : 'ONLY' ;
OPERATOR : 'OPERATOR' ;
OPTION : 'OPTION' ;
OPTIONS : 'OPTIONS' ;
OR : 'OR' ;
ORDER : 'ORDER' ;
ORDINALITY : 'ORDINALITY' ;
OTHERS : 'OTHERS' ;
OUT_P : 'OUT' ;
OUTER_P : 'OUTER' ;
OVER : 'OVER' ;
OVERLAPS : 'OVERLAPS' ;
OVERLAY : 'OVERLAY' ;
OVERRIDING : 'OVERRIDING' ;
OWNED : 'OWNED' ;
OWNER : 'OWNER' ;
PARALLEL : 'PARALLEL' ;
PARAMETER : 'PARAMETER' ;
PARSER : 'PARSER' ;
PARTIAL : 'PARTIAL' ;
PARTITION : 'PARTITION' ;
PASSING : 'PASSING' ;
PASSWORD : 'PASSWORD' ;
PLACING : 'PLACING' ;
PLANS : 'PLANS' ;
POLICY : 'POLICY' ;
POSITION : 'POSITION' ;
PRECEDING : 'PRECEDING' ;
PRECISION : 'PRECISION' ;
PREPARE : 'PREPARE' ;
PREPARED : 'PREPARED' ;
PRESERVE : 'PRESERVE' ;
PRIMARY : 'PRIMARY' ;
PRIOR : 'PRIOR' ;
PRIVILEGES : 'PRIVILEGES' ;
PROCEDURAL : 'PROCEDURAL' ;
PROCEDURE : 'PROCEDURE' ;
PROCEDURES : 'PROCEDURES' ;
PROGRAM : 'PROGRAM' ;
PUBLICATION : 'PUBLICATION' ;
QUOTE : 'QUOTE' ;
RANGE : 'RANGE' ;
READ : 'READ' ;
REAL : 'REAL' ;
REASSIGN : 'REASSIGN' ;
RECHECK : 'RECHECK' ;
RECURSIVE : 'RECURSIVE' ;
REF_P : 'REF' ;
REFERENCES : 'REFERENCES' ;
REFERENCING : 'REFERENCING' ;
REFRESH : 'REFRESH' ;
REINDEX : 'REINDEX' ;
RELATIVE_P : 'RELATIVE' ;
RELEASE : 'RELEASE' ;
RENAME : 'RENAME' ;
REPEATABLE : 'REPEATABLE' ;
REPLACE : 'REPLACE' ;
REPLICA : 'REPLICA' ;
RESET : 'RESET' ;
RESTART : 'RESTART' ;
RESTRICT : 'RESTRICT' ;
RETURN : 'RETURN' ;
RETURNING : 'RETURNING' ;
RETURNS : 'RETURNS' ;
REVOKE : 'REVOKE' ;
RIGHT : 'RIGHT' ;
ROLE : 'ROLE' ;
ROLLBACK : 'ROLLBACK' ;
ROLLUP : 'ROLLUP' ;
ROUTINE : 'ROUTINE' ;
ROUTINES : 'ROUTINES' ;
ROW : 'ROW' ;
ROWS : 'ROWS' ;
RULE : 'RULE' ;
SAVEPOINT : 'SAVEPOINT' ;
SCALAR : 'SCALAR' ;
SCHEMA : 'SCHEMA' ;
SCHEMAS : 'SCHEMAS' ;
SCROLL : 'SCROLL' ;
SEARCH : 'SEARCH' ;
SECOND_P : 'SECOND' ;
SECURITY : 'SECURITY' ;
SELECT : 'SELECT' ;
SEQUENCE : 'SEQUENCE' ;
SEQUENCES : 'SEQUENCES' ;
SERIALIZABLE : 'SERIALIZABLE' ;
SERVER : 'SERVER' ;
SESSION : 'SESSION' ;
SESSION_USER : 'SESSION_USER' ;
SET : 'SET' ;
SETOF : 'SETOF' ;
SETS : 'SETS' ;
SHARE : 'SHARE' ;
SHOW : 'SHOW' ;
SIMILAR : 'SIMILAR' ;
SIMPLE : 'SIMPLE' ;
SKIP : 'SKIP' ;
SMALLINT : 'SMALLINT' ;
SNAPSHOT : 'SNAPSHOT' ;
SOME : 'SOME' ;
SQL_P : 'SQL' ;
STABLE : 'STABLE' ;
STANDALONE_P : 'STANDALONE' ;
START : 'START' ;
STATEMENT : 'STATEMENT' ;
STATISTICS : 'STATISTICS' ;
STDIN : 'STDIN' ;
STDOUT : 'STDOUT' ;
STORAGE : 'STORAGE' ;
STORED : 'STORED' ;
STRICT_P : 'STRICT' ;
STRIP_P : 'STRIP' ;
SUBSCRIPTION : 'SUBSCRIPTION' ;
SUBSTRING : 'SUBSTRING' ;
SUPPORT : 'SUPPORT' ;
SYMMETRIC : 'SYMMETRIC' ;
SYSID : 'SYSID' ;
SYSTEM_P : 'SYSTEM' ;
SYSTEM_USER : 'SYSTEM_USER' ;
TABLE : 'TABLE' ;
TABLES : 'TABLES' ;
TABLESAMPLE : 'TABLESAMPLE' ;
TABLESPACE : 'TABLESPACE' ;
TEMP : 'TEMP' ;
TEMPLATE : 'TEMPLATE' ;
TEMPORARY : 'TEMPORARY' ;
TEXT_P : 'TEXT' ;
THEN : 'THEN' ;
TIES : 'TIES' ;
TIME : 'TIME' ;
TIMESTAMP : 'TIMESTAMP' ;
TO : 'TO' ;
TRAILING : 'TRAILING' ;
TRANSACTION : 'TRANSACTION' ;
TRANSFORM : 'TRANSFORM' ;
TREAT : 'TREAT' ;
TRIGGER : 'TRIGGER' ;
TRIM : 'TRIM' ;
TRUE_P : 'TRUE' ;
TRUNCATE : 'TRUNCATE' ;
TRUSTED : 'TRUSTED' ;
TYPE_P : 'TYPE' ;
TYPES_P : 'TYPES' ;
UESCAPE : 'UESCAPE' ;
UNBOUNDED : 'UNBOUNDED' ;
UNCOMMITTED : 'UNCOMMITTED' ;
UNENCRYPTED : 'UNENCRYPTED' ;
UNION : 'UNION' ;
UNIQUE : 'UNIQUE' ;
UNKNOWN : 'UNKNOWN' ;
UNLISTEN : 'UNLISTEN' ;
UNLOGGED : 'UNLOGGED' ;
UNTIL : 'UNTIL' ;
UPDATE : 'UPDATE' ;
USER : 'USER' ;
USING : 'USING' ;
VACUUM : 'VACUUM' ;
VALID : 'VALID' ;
VALIDATE : 'VALIDATE' ;
VALIDATOR : 'VALIDATOR' ;
VALUE_P : 'VALUE' ;
VALUES : 'VALUES' ;
VARCHAR : 'VARCHAR' ;
VARIADIC : 'VARIADIC' ;
VARYING : 'VARYING' ;
VERBOSE : 'VERBOSE' ;
VERSION_P : 'VERSION' ;
VIEW : 'VIEW' ;
VIEWS : 'VIEWS' ;
VOLATILE : 'VOLATILE' ;
WHEN : 'WHEN' ;
WHERE : 'WHERE' ;
WHITESPACE_P : 'WHITESPACE' ;
WINDOW : 'WINDOW' ;
WITH : 'WITH' ;
WITHIN : 'WITHIN' ;
WITHOUT : 'WITHOUT' ;
WORK : 'WORK' ;
WRAPPER : 'WRAPPER' ;
WRITE : 'WRITE' ;
XML_P : 'XML' ;
XMLATTRIBUTES : 'XMLATTRIBUTES' ;
XMLCONCAT : 'XMLCONCAT' ;
XMLELEMENT : 'XMLELEMENT' ;
XMLEXISTS : 'XMLEXISTS' ;
XMLFOREST : 'XMLFOREST' ;
XMLNAMESPACES : 'XMLNAMESPACES' ;
XMLPARSE : 'XMLPARSE' ;
XMLPI : 'XMLPI' ;
XMLROOT : 'XMLROOT' ;
XMLSERIALIZE : 'XMLSERIALIZE' ;
XMLTABLE : 'XMLTABLE' ;
YEAR_P : 'YEAR' ;
YES_P : 'YES' ;
ZONE : 'ZONE' ;


LESS_EQUALS : '<=' ;
GREATER_EQUALS : '>=' ;
NOT_EQUALS : "!=|<>" ;
COLON_EQUALS : ':=' ;
EQUALS_GREATER : ' =>' ;
TYPECAST : '::' ;
//DOT_DOT : '..' ;

IDENT : "[A-Za-z\200-\377_][A-Za-z\200-\377_0-9\$]*" ;
FCONST : "(([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*))" ;
SCONST : "\"(\\[\"\\]|[^\"\n])*\"|'(\\['\\]|[^'\n])*'|\$\$[^\$]+\$\$" ;
BCONST : 'BCONST' ;
XCONST : 'XCONST' ;
ICONST : "[0-9]+" ;
PARAM : "\$[0-9]+";

MODE_TYPE_NAME : 'MODE_TYPE_NAME' ;
MODE_PLPGSQL_EXPR : 'MODE_PLPGSQL_EXPR' ;
MODE_PLPGSQL_ASSIGN1 : 'MODE_PLPGSQL_ASSIGN1' ;
MODE_PLPGSQL_ASSIGN2 : 'MODE_PLPGSQL_ASSIGN2' ;
MODE_PLPGSQL_ASSIGN3 : 'MODE_PLPGSQL_ASSIGN3' ;

FORMAT_LA : 'FROMAT_LA' ;
WITH_LA : 'WITH_LA' ;
WITHOUT_LA : 'WITHOUT_LA' ;
NULLS_LA : 'NULLS_LA';
NOT_LA : 'NOT_LA' ;
Op : 'Op' ;

}