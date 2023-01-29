--/*
-- * Copyright (c) Meta Platforms, Inc. AND affiliates.
-- *
-- * This source code is licensed under the MIT license found in the
-- * LICENSE file in the root directory of this source tree.
-- */

DECLARE PROC printf NO CHECK;
DECLARE PROC puts NO CHECK;

-- basic test TABLE WITH an auto inc field
CREATE TABLE foo(
  id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL
);

-- second test TABLE WITH combination of fields
@attribute(bar_is_good=1)
CREATE TABLE bar(
  id INTEGER NOT NULL PRIMARY KEY,
  @attribute(collossal_cave='xyzzy')
  name TEXT,
  rate LONG INT,
  type INTEGER,
  size REAL @create(2)
);

-- test VIEW that reads FROM the test tables
CREATE VIEW baz AS SELECT id, name, type FROM bar;

-- DECLARE variables of the basic types
DECLARE i0_NULLable int;
DECLARE i1_NULLable int;
DECLARE r0_NULLable real;
DECLARE l0_NULLable long INTEGER;
DECLARE l1_NULLable long INTEGER;
DECLARE b0_NULLable bool;
DECLARE t0_NULLable text;

-- same types but NOT NULL variant
DECLARE i2 int NOT NULL;
DECLARE r2 real NOT NULL;
DECLARE l2 long INTEGER NOT NULL;
DECLARE b2 bool NOT NULL;
DECLARE t2 text NOT NULL;

-- initialize FOR later use
SET t2 := "text";

-- TEST: assign eveything NOT NULL
-- Note: semantic analysis verifies no chance of
--       assigning NULLable to NOT NULLable
-- + i2 = 1;
SET i2 := 1;

-- TEST: assign rhs NOT NULL
-- + cql_set_notNULL(i1_NULLable, 88);
SET i1_NULLable := 88;

-- remove the NULLability improvement
-- + cql_set_NULL(i1_NULLable);
SET i1_NULLable := NULL;

-- TEST: assign everything NULLable
-- + cql_set_NULLable(i0_NULLable, i1_NULLable.is_NULL, i1_NULLable.value);
SET i0_NULLable := i1_NULLable;

-- TEST: assign NULL to NULLable string
-- + cql_set_string_ref(&t0_NULLable, NULL);
SET t0_NULLable := NULL;

-- + cql_set_string_ref(&t0_NULLable, t2);
SET t0_NULLable := t2;

-- remove the NULLability improvement
-- + cql_set_string_ref(&t0_NULLable, NULL);
SET t0_NULLable := NULL;

-- TEST: simple unary operators
-- + SET i2 := - -1;
-- + i2 = - - 1;
SET i2 := - -1;

-- + cql_set_notNULL(i0_NULLable, - i2);
SET i0_NULLable := -i2;

-- + cql_set_NULL(i0_NULLable);
SET i0_NULLable := -NULL;

-- + cql_set_NULLable(i1_NULLable, i0_NULLable.is_NULL, - i0_NULLable.value);
SET i1_NULLable := -i0_NULLable;

-- + cql_set_notNULL(r0_NULLable, 2.2%);
SET r0_NULLable := 2.2;

-- remove the NULLability improvement
-- + cql_set_NULL(r0_NULLable);
SET r0_NULLable := NULL;

-- + r2 = 3.5%;
SET r2 := 3.5;

-- + cql_set_NULLable(_tmp_n_bool_1, i0_NULLable.is_NULL, ! i0_NULLable.value);
-- + cql_set_NULLable(_tmp_n_bool_0, _tmp_n_bool_1.is_NULL, ! _tmp_n_bool_1.value);
-- + cql_set_NULLable(i1_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i1_NULLable := NOT NOT i0_NULLable;

-- + i2 = ! ! b2;
SET i2 := NOT NOT b2;

-- TEST: NOT NULL arithmetic
-- + i2 = 1 * 3 + 5;
SET i2 := 1 * 3 + 5;

-- TEST: everything in sight is NULLable
-- + cql_combine_NULLables(r0_NULLable, r0_NULLable.is_NULL, i1_NULLable.is_NULL, r0_NULLable.value * i1_NULLable.value);
SET r0_NULLable := r0_NULLable * i1_NULLable;

-- TEST: right operand is NOT NULL
-- + cql_set_NULLable(r0_NULLable, r0_NULLable.is_NULL, r0_NULLable.value * i2);
SET r0_NULLable := r0_NULLable * i2;

-- TEST: left operand is NOT NULL
-- + cql_set_NULLable(i0_NULLable, i1_NULLable.is_NULL, 12 * i1_NULLable.value);
SET i0_NULLable := 12 * i1_NULLable;

-- TEST: an operaand is actually NULL
-- + cql_set_NULL(i0_NULLable);
SET i0_NULLable := NULL * i1_NULLable;

-- TEST: make sure the stacking is working correctly
-- + cql_combine_NULLables(_tmp_n_double_1, r0_NULLable.is_NULL, i1_NULLable.is_NULL, r0_NULLable.value * i1_NULLable.value);
-- + cql_combine_NULLables(_tmp_n_double_2, r0_NULLable.is_NULL, i1_NULLable.is_NULL, r0_NULLable.value * i1_NULLable.value);
-- + cql_combine_NULLables(r0_NULLable, _tmp_n_double_1.is_NULL, _tmp_n_double_2.is_NULL, _tmp_n_double_1.value + _tmp_n_double_2.value);
SET r0_NULLable := r0_NULLable * i1_NULLable + r0_NULLable * i1_NULLable;

-- TEST: a more complex stacking example
-- + cql_combine_NULLables(_tmp_n_double_2, r0_NULLable.is_NULL, i1_NULLable.is_NULL, r0_NULLable.value * i1_NULLable.value);
-- + cql_combine_NULLables(_tmp_n_double_3, r0_NULLable.is_NULL, i0_NULLable.is_NULL, r0_NULLable.value * i0_NULLable.value);
-- + cql_combine_NULLables(_tmp_n_double_1, _tmp_n_double_2.is_NULL, _tmp_n_double_3.is_NULL, _tmp_n_double_2.value + _tmp_n_double_3.value);
-- + cql_combine_NULLables(r0_NULLable, _tmp_n_double_1.is_NULL, r0_NULLable.is_NULL, _tmp_n_double_1.value + r0_NULLable.value);
SET r0_NULLable := (r0_NULLable * i1_NULLable + r0_NULLable * i0_NULLable) + r0_NULLable;

-- TEST: string assignment -- nasty string
-- + cql_set_string_ref(&t2, _literal%This_is_a_test_);
SET t2 := "This is a \" \\ test '' \n \" ";

-- TEST: call an external procedure (type not known)
-- + printf("Hello, world\n");
call printf("Hello, world\n");

-- TEST: logical AND WITH short circuit
-- + i2 = r2 && l2;
SET i2 := r2 AND l2;

-- helper methods FOR the next test
DECLARE FUNCTION side_effect1() INTEGER;
DECLARE FUNCTION side_effect2() INTEGER;

-- TEST: the operands have side effects, the short circuit must not
-- do the evaluation of the side effect FOR the second arg IF the first
-- returns false.  This is the trickiest case because ti looks LIKE it's
-- safe  to use the (x && y) form because the operands are non-NULL.
-- it isn't though because there was expression work to get to the non-NULL
-- state.  The Coalesce is important to this test FOR that reason.
-- +  do {
-- +    _tmp_n_int_2 = side_effect1();
-- +    IF (!_tmp_n_int_2.is_NULL) {
-- +      _tmp_int_1 = _tmp_n_int_2.value;
-- +      break;
-- +    }
-- +    _tmp_int_1 = 7;
-- +  } WHILE (0);
-- +  IF (!(_tmp_int_1)) {
-- +    _tmp_bool_0 = 0;
-- +  }
-- +  else {
-- +      _tmp_n_int_3 = side_effect2();
-- +      IF (!_tmp_n_int_3.is_NULL) {
-- +        _tmp_int_2 = _tmp_n_int_3.value;
-- +        break;
-- +      }
-- +      _tmp_int_2 = 5;
-- +    } WHILE (0);
-- +    _tmp_bool_0 = !!(_tmp_int_2);
-- +  }
-- +  i2 = _tmp_bool_0;
SET i2 := coalesce(side_effect1(), 7) AND coalesce(side_effect2(), 5);

-- TEST: trival NULL on AND
-- + cql_set_NULL(_tmp_n_bool_0);
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := NULL AND NULL;

-- TEST: logical AND WITH NULLables
-- + IF (cql_is_NULLable_false(i0_NULLable.is_NULL, i0_NULLable.value))
-- + IF (cql_is_NULLable_false(i1_NULLable.is_NULL, i1_NULLable.value))
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := i0_NULLable AND i1_NULLable;

-- TEST: logical AND WITH constant NULLs
-- + IF (cql_is_NULLable_false(i1_NULLable.is_NULL, i1_NULLable.value))
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := NULL AND i1_NULLable;

-- TEST: logical AND WITH constant NULLs
-- + IF (cql_is_NULLable_false(i0_NULLable.is_NULL, i0_NULLable.value))
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := i0_NULLable AND NULL;

-- TEST: logical OR WITH short circuit
-- + i2 = r2 || l2;
SET i2 := r2 OR l2;

-- TEST: complex side effect, looks safe but it isn't because of codegen
-- +  do {
-- +    _tmp_n_int_2 = side_effect1();
-- +    IF (!_tmp_n_int_2.is_NULL) {
-- +      _tmp_int_1 = _tmp_n_int_2.value;
-- +      break;
-- +    }
-- +    _tmp_int_1 = 7;
-- +  } WHILE (0);
-- +  IF (_tmp_int_1) {
-- +    _tmp_bool_0 = 1;
-- +  }
-- +  else {
-- +    do {
-- +      _tmp_n_int_3 = side_effect2();
-- +      IF (!_tmp_n_int_3.is_NULL) {
-- +        _tmp_int_2 = _tmp_n_int_3.value;
-- +        break;
-- +      }
-- +      _tmp_int_2 = 5;
-- +    } WHILE (0);
-- +    _tmp_bool_0 = !!(_tmp_int_2);
-- +  }
-- +  i2 = _tmp_bool_0;
SET i2 := coalesce(side_effect1(), 7) OR coalesce(side_effect2(), 5);

-- TEST: trival NULL on OR
-- + cql_set_NULL(_tmp_n_bool_0);
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := NULL OR NULL;

-- TEST: logical OR WITH NULLables
-- + cql_is_NULLable_true(i0_NULLable.is_NULL, i0_NULLable.value)
-- + cql_is_NULLable_true(i1_NULLable.is_NULL, i1_NULLable.value)
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := i0_NULLable OR i1_NULLable;

-- TEST: logical OR WITH constant NULLs
-- + cql_is_NULLable_true(i1_NULLable.is_NULL, i1_NULLable.value)
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := NULL OR i1_NULLable;

-- TEST: logical OR WITH constant NULLs
-- + cql_is_NULLable_true(i0_NULLable.is_NULL, i0_NULLable.value)
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := i0_NULLable OR NULL;

-- TEST: is NULL basic test
-- + i2 = 1;
SET i2 := NULL is NULL;

-- TEST: is NULL test general case
-- + cql_combine_NULLables(_tmp_n_int_0, i0_NULLable.is_NULL, i1_NULLable.is_NULL, i0_NULLable.value + i1_NULLable.value);
-- + i2 = _tmp_n_int_0.is_NULL;
SET i2 := (i0_NULLable + i1_NULLable) is NULL;

-- TEST: is NOT NULL basic test
-- + i2 = !1;
SET i2 := NULL is NOT NULL;

-- TEST: is NOT NULL test general case
-- + cql_combine_NULLables(_tmp_n_int_0, i0_NULLable.is_NULL, i1_NULLable.is_NULL, i0_NULLable.value + i1_NULLable.value);
-- + i2 = !_tmp_n_int_0.is_NULL;
SET i2 := (i0_NULLable + i1_NULLable) is NOT NULL;

-- TEST: complex if/else pattern
-- Note: of interest because the embedded NULLable
--       comparison requires statements to compute
-- + IF (1) {
-- + i2 = 1;
-- + }
-- + else {
-- + cql_combine_NULLables(_tmp_n_bool_0, i0_NULLable.is_NULL, i1_NULLable.is_NULL, i0_NULLable.value == i1_NULLable.value);
-- + IF (cql_is_NULLable_true(_tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value)) {
-- + i2 = 2;
-- + }
-- + else {
-- +  i2 = 3;
-- + }
-- + }
-- +4 {
-- +4 }
-- +2 else {
if 1 then
 SET i2 := 1;
else IF i0_NULLable == i1_NULLable then
 SET i2 := 2;
else
 SET i2 := 3;
end if;

-- TEST: complex if/else pattern, embedded logical operation
-- Note: of interest because the embedded logical requires
--       statements to compute
-- validating the bits are are unique to this construct
-- + IF (cql_is_NULLable_true(i0_NULLable.is_NULL, i0_NULLable.value)) {
-- + IF (cql_is_NULLable_true(i1_NULLable.is_NULL, i1_NULLable.value)) {
-- + IF (cql_is_NULLable_true(_tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value)) {
if 1 then
 SET i2 := 1;
else IF i0_NULLable OR i1_NULLable then
 SET i2 := 2;
else
 SET i2 := 3;
end if;

-- TEST: simple procedure WITH external call
-- + void test(cql_int32 i) {
-- + IF (i) {
-- + puts("true");
CREATE procedure test(i INTEGER NOT NULL)
BEGIN
  IF i then
    call puts('true');
  end if;
END;

-- TEST: guard statements are simply rewritten to IF statements
-- + IF (!a.is_NULL) {
-- + goto cql_cleanup; // return
-- + cql_set_NULLable(x, a.is_NULL, a.value);
CREATE PROC proc_with_return_guard(a int)
BEGIN
  IF a is NOT NULL return;
  LET x := a;
END;

-- TEST: simple between
-- + SET b2 := BETWEEN REWRITE _between_0_ := 1 CHECK (_between_0_ >= 0 AND _between_0_ <= 3);
-- + _between_0_ = 1;
-- + b2 = _between_0_ >= 0 && _between_0_ <= 3;
SET b2 := 1 between 0 AND 3;

-- TEST: between WITH some NULLables
-- + SET i0_NULLable := BETWEEN REWRITE _between_1_ := i1_NULLable CHECK (_between_1_ >= i0_NULLable AND _between_1_ <= r2);
-- + cql_set_NULLable(_between_1_, i1_NULLable.is_NULL, i1_NULLable.value);
-- + cql_combine_NULLables(_tmp_n_bool_1, _between_1_.is_NULL, i0_NULLable.is_NULL, _between_1_.value >= i0_NULLable.value);
-- + IF (cql_is_NULLable_false(_tmp_n_bool_1.is_NULL, _tmp_n_bool_1.value)) {
-- +   cql_set_notNULL(_tmp_n_bool_0, 0);
-- + }
-- + else {
-- +   cql_set_NULLable(_tmp_n_bool_2, _between_1_.is_NULL, _between_1_.value <= r2);
-- +   IF (cql_is_NULLable_false(_tmp_n_bool_2.is_NULL, _tmp_n_bool_2.value)) {
-- +     cql_set_notNULL(_tmp_n_bool_0, 0);
-- +   }
-- +   else {
-- +     cql_combine_NULLables(_tmp_n_bool_0, _tmp_n_bool_1.is_NULL, _tmp_n_bool_2.is_NULL, 1);
-- +   }
-- + }
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := i1_NULLable between i0_NULLable AND r2;

-- TEST: between WITH different NULLables
-- + SET i0_NULLable := BETWEEN REWRITE _between_2_ := i1_NULLable CHECK (_between_2_ >= r2 AND _between_2_ <= i0_NULLable);
-- + cql_set_NULLable(_between_2_, i1_NULLable.is_NULL, i1_NULLable.value);
-- + cql_set_NULLable(_tmp_n_bool_1, _between_2_.is_NULL, _between_2_.value >= r2);
-- + IF (cql_is_NULLable_false(_tmp_n_bool_1.is_NULL, _tmp_n_bool_1.value)) {
-- +   cql_set_notNULL(_tmp_n_bool_0, 0);
-- + }
-- + else {
-- +   cql_combine_NULLables(_tmp_n_bool_2, _between_2_.is_NULL, i0_NULLable.is_NULL, _between_2_.value <= i0_NULLable.value);
-- +   IF (cql_is_NULLable_false(_tmp_n_bool_2.is_NULL, _tmp_n_bool_2.value)) {
-- +     cql_set_notNULL(_tmp_n_bool_0, 0);
-- +   }
-- +   else {
-- +     cql_combine_NULLables(_tmp_n_bool_0, _tmp_n_bool_1.is_NULL, _tmp_n_bool_2.is_NULL, 1);
-- +   }
-- + }
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := i1_NULLable between r2 AND i0_NULLable;

-- TEST: simple not between
-- + SET b2 := BETWEEN REWRITE _between_3_ := 1 CHECK (_between_3_ < 0 OR _between_3_ > 3);
-- + _between_3_ = 1;
-- + b2 = _between_3_ < 0 || _between_3_ > 3;
SET b2 := 1 not between 0 AND 3;

-- TEST: not between WITH some NULLables
-- + SET i0_NULLable := BETWEEN REWRITE _between_4_ := i1_NULLable CHECK (_between_4_ < i0_NULLable OR _between_4_ > r2);
-- + cql_set_NULLable(_between_4_, i1_NULLable.is_NULL, i1_NULLable.value);
-- + cql_combine_NULLables(_tmp_n_bool_1, _between_4_.is_NULL, i0_NULLable.is_NULL, _between_4_.value < i0_NULLable.value);
-- + IF (cql_is_NULLable_true(_tmp_n_bool_1.is_NULL, _tmp_n_bool_1.value)) {
-- +   cql_set_notNULL(_tmp_n_bool_0, 1);
-- + }
-- + else {
-- +   cql_set_NULLable(_tmp_n_bool_2, _between_4_.is_NULL, _between_4_.value > r2);
-- +   IF (cql_is_NULLable_true(_tmp_n_bool_2.is_NULL, _tmp_n_bool_2.value)) {
-- +     cql_set_notNULL(_tmp_n_bool_0, 1);
-- +   }
-- +   else {
-- +     cql_combine_NULLables(_tmp_n_bool_0, _tmp_n_bool_1.is_NULL, _tmp_n_bool_2.is_NULL, 0);
-- +   }
-- + }
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := i1_NULLable not between i0_NULLable AND r2;

-- TEST: not between WITH different NULLables
-- + SET i0_NULLable := BETWEEN REWRITE _between_5_ := i1_NULLable CHECK (_between_5_ < r2 OR _between_5_ > i0_NULLable);
-- + cql_set_NULLable(_between_5_, i1_NULLable.is_NULL, i1_NULLable.value);
-- + cql_set_NULLable(_tmp_n_bool_1, _between_5_.is_NULL, _between_5_.value < r2);
-- + IF (cql_is_NULLable_true(_tmp_n_bool_1.is_NULL, _tmp_n_bool_1.value)) {
-- +   cql_set_notNULL(_tmp_n_bool_0, 1);
-- + }
-- + else {
-- +   cql_combine_NULLables(_tmp_n_bool_2, _between_5_.is_NULL, i0_NULLable.is_NULL, _between_5_.value > i0_NULLable.value);
-- +   IF (cql_is_NULLable_true(_tmp_n_bool_2.is_NULL, _tmp_n_bool_2.value)) {
-- +     cql_set_notNULL(_tmp_n_bool_0, 1);
-- +   }
-- +   else {
-- +     cql_combine_NULLables(_tmp_n_bool_0, _tmp_n_bool_1.is_NULL, _tmp_n_bool_2.is_NULL, 0);
-- +   }
-- + }
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := i1_NULLable not between r2 AND i0_NULLable;

-- TEST: out parameter test
-- + void out_test(cql_int32 *_NonNULL i, cql_NULLable_int32 *_NonNULL ii) {
-- + *i = i2;
-- + cql_set_NULLable(*ii, i0_NULLable.is_NULL, i0_NULLable.value);
-- + }
CREATE procedure out_test(out i INTEGER NOT NULL, out ii INTEGER)
BEGIN
  SET i := i2;
  SET ii := i0_NULLable;
END;

-- TEST: force a cql_int64 variable to be pushed on the scratch stack
-- + cql_NULLable_int64 longint_var = { .is_NULL = 1 };
DECLARE longint_var long INTEGER;

-- + cql_combine_NULLables(_tmp_n_int64_1, l0_NULLable.is_NULL, l1_NULLable.is_NULL, l0_NULLable.value + l1_NULLable.value);
-- + cql_set_NULLable(longint_var, _tmp_n_int64_1.is_NULL, _tmp_n_int64_1.value * 5);
SET longint_var := (l0_NULLable + l1_NULLable) * 5;

-- TEST: make a cursor
-- + _rc_ = cql_prepare(_db_, &foo_cursor_stmt,
-- + "SELECT id, ? "
-- + "FROM foo "
-- + "WHERE id = ?"
-- + cql_multibind(&_rc_, _db_, &foo_cursor_stmt, 2,
-- +               CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, i2,
-- +               CQL_DATA_TYPE_INT32, &i0_NULLable);
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
DECLARE foo_cursor CURSOR FOR SELECT id, i2 FROM foo WHERE id = i0_NULLable;

-- TEST: fetch a cursor
-- + _rc_ = sqlite3_step(foo_cursor_stmt);
-- + _rc_ = sqlite3_step(foo_cursor_stmt);
-- + _foo_cursor_has_row_ = _rc_ == SQLITE_ROW;
-- + cql_multifetch(_rc_, foo_cursor_stmt, 2,
-- +                CQL_DATA_TYPE_INT32, &i0_NULLable,
-- +                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &i2);
-- + IF (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
fetch foo_cursor into i0_NULLable, i2;

-- TEST: test elementary CURSOR on SELECT WITH no tables, still round trips through sqlite
DECLARE col1 INTEGER;
DECLARE col2 real NOT NULL;
-- + _rc_ = cql_prepare(_db_, &basic_cursor_stmt,
DECLARE basic_cursor CURSOR FOR SELECT 1, 2.5;
-- + cql_multifetch(_rc_, basic_cursor_stmt, 2,
-- +                CQL_DATA_TYPE_INT32, &col1,
-- +                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_DOUBLE, &col2);
fetch basic_cursor into col1, col2;
-- + cql_finalize_stmt(&basic_cursor_stmt);
close basic_cursor;

-- TEST: the most expensive way to swap two variables ever :)
DECLARE arg1 INTEGER NOT NULL;
DECLARE arg2 INTEGER NOT NULL;
SET arg1 := 7;
SET arg2 := 11;
-- + _rc_ = cql_prepare(_db_, &exchange_cursor_stmt,
-- + cql_multibind(&_rc_, _db_, &exchange_cursor_stmt, 2,
-- +               CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, arg2,
-- +               CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, arg1);
DECLARE exchange_cursor CURSOR FOR SELECT arg2, arg1;
-- + cql_multifetch(_rc_, exchange_cursor_stmt, 2,
-- +                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &arg1,
-- +                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &arg2);
fetch exchange_cursor into arg1, arg2;
-- + cql_finalize_stmt(&exchange_cursor_stmt);
close exchange_cursor;

-- TEST: simple nested select
-- + _rc_ = cql_prepare(_db_, &_temp_stmt,
-- +  "SELECT ? + 1"
-- + cql_multibind(&_rc_, _db_, &_temp_stmt, 1,
-- +               CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, i2);
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
-- + _rc_ = sqlite3_step(_temp_stmt);
-- + IF (_rc_ != SQLITE_ROW) { cql_error_trace(); goto cql_cleanup; }
-- + i2 = sqlite3_column_int(_temp_stmt, 0);
-- + cql_finalize_stmt(&_temp_stmt);
SET i2 := (SELECT i2+1);

-- TEST: nested SELECT WITH NULLable
-- validate just the different bit
-- + cql_multibind(&_rc_, _db_, &_temp_stmt, 1,
-- +               CQL_DATA_TYPE_INT32, &i0_NULLable);
SET i0_NULLable := (SELECT i0_NULLable+1);

-- TEST: tricky quoted text
-- this validates that the C escaping works right when making SQL
-- + "DELETE FROM bar WHERE name LIKE '\\\\ \" \\n'"
DELETE FROM bar WHERE name LIKE '\\ " \n';

-- TEST: binding an out parameter
-- + cql_multibind(&_rc_, _db_, &_temp_stmt, 1,
-- +               CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, *foo);
CREATE procedure outparm_test(out foo INTEGER NOT NULL)
BEGIN
 SET foo := 1;
 DELETE FROM bar WHERE id = foo;
END;

-- TEST: a simple stored PROC that throws
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto catch_start_1; }
-- + goto catch_end_1;
-- + catch_start_1: {
-- + printf("error\n");
-- + _rc_ = cql_best_error(_rc_thrown_1);
-- + goto cql_cleanup;
-- + catch_end_1:
-- + _rc_ = SQLITE_OK;
-- + cql_cleanup:
CREATE procedure throwing()
BEGIN
  BEGIN try
   DELETE FROM bar;
  end try;
  BEGIN catch
   call printf("error\n");
   throw;
  end catch;
END;

-- TEST: a simple case expression
-- + do {
-- +  IF (1) {
-- +   i2 = 100;
-- +   break;
-- +  }
-- +  IF (2) {
-- +   i2 = 200;
-- +   break;
-- +  }
-- +  i2 = 300;
-- + } WHILE (0);
SET i2 := case when 1 then 100 when 2 then 200 when NULL then 500 else 300 END;

-- TEST: a simple in expression
-- + do {
-- +  _tmp_int_% = 3;
-- +  _tmp_bool_0 = 1;
-- +  IF (_tmp_int_% == 1) break;
-- +  IF (_tmp_int_% == 2) break;
-- +  IF (_tmp_int_% == 4) break;
-- +  _tmp_bool_0 = 0;
-- + } WHILE (0);
-- + i2 = _tmp_bool_0;
SET i2 := 3 in (1, 2, NULL, 4);

-- TEST: in WITH NULLables
-- + do {
-- +  cql_set_NULLable(_tmp_n_int_%, i1_NULLable.is_NULL, i1_NULLable.value);
-- +  IF (_tmp_n_int_%.is_NULL) {
-- +    cql_set_NULL(_tmp_n_bool_0);
-- +    break;
-- +  }
-- +  cql_set_notNULL(_tmp_n_bool_0, 1);
-- +  IF (_tmp_n_int_%.value == 1) break;
-- +  IF (_tmp_n_int_%.value == 2) break;
-- +  IF (cql_is_NULLable_true(b0_NULLable.is_NULL, _tmp_n_int_%.value == b0_NULLable.value)) break;
-- +  cql_set_notNULL(_tmp_n_bool_0, 0);
-- + } WHILE (0);
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := i1_NULLable in (1, 2, NULL, b0_NULLable);

-- TEST: a simple not in expression
-- + do {
-- +  _tmp_int_% = 3;
-- +  _tmp_bool_0 = 0;
-- +  IF (_tmp_int_% == 1) break;
-- +  IF (_tmp_int_% == 2) break;
-- +  IF (_tmp_int_% == 4) break;
-- +  _tmp_bool_0 = 1;
-- + } WHILE (0);
-- + i2 = _tmp_bool_0;
SET i2 := 3 not in (1, 2, NULL, 4);

-- TEST: not in WITH NULLables
-- + do {
-- +  cql_set_NULLable(_tmp_n_int_%, i1_NULLable.is_NULL, i1_NULLable.value);
-- +  IF (_tmp_n_int_%.is_NULL) {
-- +    cql_set_NULL(_tmp_n_bool_0);
-- +    break;
-- +  }
-- +  cql_set_notNULL(_tmp_n_bool_0, 0);
-- +  IF (_tmp_n_int_%.value == 1) break;
-- +  IF (_tmp_n_int_%.value == 2) break;
-- +  IF (cql_is_NULLable_true(b0_NULLable.is_NULL, _tmp_n_int_%.value == b0_NULLable.value)) break;
-- +  cql_set_notNULL(_tmp_n_bool_0, 1);
-- + } WHILE (0);
-- + cql_set_NULLable(i0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET i0_NULLable := i1_NULLable not in (1, 2, NULL, b0_NULLable);

-- TEST: between WITH strings
-- + SET b2 := BETWEEN REWRITE _between_6_ := 'b' CHECK (_between_6_ >= 'a' AND _between_6_ <= 'c');
-- + cql_set_string_ref(&_between_6_, _literal_%_b_);
-- + b2 = cql_string_compare(_between_6_, _literal_%_a_) >= 0 && cql_string_compare(_between_6_, _literal_%_c_) <= 0;
SET b2 := 'b' between 'a' AND 'c';

-- TEST: between WITH NULLable strings right
-- + SET b0_NULLable := BETWEEN REWRITE _between_7_ := 'b' CHECK (_between_7_ >= 'a' AND _between_7_ <= t0_NULLable);
-- + cql_set_string_ref(&_between_7_, _literal_%_b_);
-- + IF (!(cql_string_compare(_between_7_, _literal_%_a_) >= 0)) {
-- +   cql_set_notNULL(_tmp_n_bool_0, 0);
-- + }
-- + else {
-- +   cql_combine_NULLables(_tmp_n_bool_2, !_between_7_, !t0_NULLable, cql_string_compare(_between_7_, t0_NULLable) <= 0);
-- +   IF (cql_is_NULLable_false(_tmp_n_bool_2.is_NULL, _tmp_n_bool_2.value)) {
-- +     cql_set_notNULL(_tmp_n_bool_0, 0);
-- +   }
-- +   else {
-- +     cql_set_NULLable(_tmp_n_bool_0, _tmp_n_bool_2.is_NULL, 1);
-- +   }
-- + }
-- + cql_set_NULLable(b0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET b0_NULLable := 'b' between 'a' AND t0_NULLable;

-- TEST: between WITH NULLable strings left
-- + SET b0_NULLable := BETWEEN REWRITE _between_8_ := 'b' CHECK (_between_8_ >= t0_NULLable AND _between_8_ <= 'c');
-- + cql_set_string_ref(&_between_8_, _literal_%_b_);
-- + cql_combine_NULLables(_tmp_n_bool_1, !_between_8_, !t0_NULLable, cql_string_compare(_between_8_, t0_NULLable) >= 0);
-- + IF (cql_is_NULLable_false(_tmp_n_bool_1.is_NULL, _tmp_n_bool_1.value)) {
-- +   cql_set_notNULL(_tmp_n_bool_0, 0);
-- + }
-- + else {
-- +   IF (!(cql_string_compare(_between_8_, _literal_%_c_) <= 0)) {
-- +     cql_set_notNULL(_tmp_n_bool_0, 0);
-- +   }
-- +   else {
-- +     cql_set_NULLable(_tmp_n_bool_0, _tmp_n_bool_1.is_NULL, 1);
-- +   }
-- + }
-- + cql_set_NULLable(b0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET b0_NULLable := 'b' between t0_NULLable AND 'c';

-- TEST: between WITH NULLable strings NULL operand
-- + SET b0_NULLable := BETWEEN REWRITE _between_9_ := 'b' CHECK (_between_9_ >= NULL AND _between_9_ <= 'c');
-- + cql_set_string_ref(&_between_9_, _literal_%_b_);
-- + cql_set_NULL(_tmp_n_bool_1);
-- + IF (cql_is_NULLable_false(_tmp_n_bool_1.is_NULL, _tmp_n_bool_1.value)) {
-- +   cql_set_notNULL(_tmp_n_bool_0, 0);
-- + }
-- + else {
-- +   IF (!(cql_string_compare(_between_9_, _literal_%_c_) <= 0)) {
-- +     cql_set_notNULL(_tmp_n_bool_0, 0);
-- +   }
-- +   else {
-- +     cql_set_NULLable(_tmp_n_bool_0, _tmp_n_bool_1.is_NULL, 1);
-- +   }
-- + }
-- + cql_set_NULLable(b0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET b0_NULLable := 'b' between NULL AND 'c';

-- TEST: not between WITH strings
-- + SET b2 := BETWEEN REWRITE _between_10_ := 'b' CHECK (_between_10_ < 'a' OR _between_10_ > 'c');
-- + cql_set_string_ref(&_between_10_, _literal_%_b_);
-- + b2 = cql_string_compare(_between_10_, _literal_%_a_) < 0 || cql_string_compare(_between_10_, _literal_%_c_) > 0;
SET b2 := 'b' not between 'a' AND 'c';

-- TEST: not between WITH NULLable strings right
-- + SET b0_NULLable := BETWEEN REWRITE _between_11_ := 'b' CHECK (_between_11_ < 'a' OR _between_11_ > t0_NULLable);
-- + cql_set_string_ref(&_between_11_, _literal_%_b_);
-- + IF (cql_string_compare(_between_11_, _literal_%_a_) < 0) {
-- +   cql_set_notNULL(_tmp_n_bool_0, 1);
-- + }
-- + else {
-- +   cql_combine_NULLables(_tmp_n_bool_2, !_between_11_, !t0_NULLable, cql_string_compare(_between_11_, t0_NULLable) > 0);
-- +   IF (cql_is_NULLable_true(_tmp_n_bool_2.is_NULL, _tmp_n_bool_2.value)) {
-- +     cql_set_notNULL(_tmp_n_bool_0, 1);
-- +   }
-- +   else {
-- +      cql_set_NULLable(_tmp_n_bool_0, _tmp_n_bool_2.is_NULL, 0);
-- +   }
-- + }
-- + cql_set_NULLable(b0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET b0_NULLable := 'b' not between 'a' AND t0_NULLable;

-- TEST: not between WITH NULLable strings left
-- + SET b0_NULLable := BETWEEN REWRITE _between_12_ := 'b' CHECK (_between_12_ < t0_NULLable OR _between_12_ > 'c');
-- + cql_set_string_ref(&_between_12_, _literal_%_b_);
-- + cql_combine_NULLables(_tmp_n_bool_1, !_between_12_, !t0_NULLable, cql_string_compare(_between_12_, t0_NULLable) < 0);
-- + IF (cql_is_NULLable_true(_tmp_n_bool_1.is_NULL, _tmp_n_bool_1.value)) {
-- +   cql_set_notNULL(_tmp_n_bool_0, 1);
-- + }
-- + else {
-- +   IF (cql_string_compare(_between_12_, _literal_%_c_) > 0) {
-- +      cql_set_notNULL(_tmp_n_bool_0, 1);
-- +   }
-- +   else {
-- +     cql_set_NULLable(_tmp_n_bool_0, _tmp_n_bool_1.is_NULL, 0);
-- +   }
-- + }
-- + cql_set_NULLable(b0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET b0_NULLable := 'b' not between t0_NULLable AND 'c';

-- TEST: not between WITH NULLable strings NULL operand
-- verify rewrite
-- + SET b0_NULLable := BETWEEN REWRITE _between_13_ := 'b' CHECK (_between_13_ < NULL OR _between_13_ > 'c');
-- + cql_set_string_ref(&_between_%, _literal_%_b_);
-- + cql_set_NULL(_tmp_n_bool_1);
-- + IF (cql_is_NULLable_true(_tmp_n_bool_1.is_NULL, _tmp_n_bool_1.value)) {
-- +   cql_set_notNULL(_tmp_n_bool_0, 1);
-- + }
-- + else {
-- +   IF (cql_string_compare(_between_%_, _literal_%_c_) > 0) {
-- +     cql_set_notNULL(_tmp_n_bool_0, 1);
-- +   }
-- +   else {
-- +     cql_set_NULLable(_tmp_n_bool_0, _tmp_n_bool_1.is_NULL, 0);
-- +   }
-- + }
-- + cql_set_NULLable(b0_NULLable, _tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value);
SET b0_NULLable := 'b' not between NULL AND 'c';

-- TEST: this procedure will have a structured semantic type
-- + cql_string_proc_name(with_result_set_stored_procedure_name, "with_result_set");
-- + cql_code with_result_set(sqlite3 *_NonNULL _db_, sqlite3_stmt *_NULLable *_NonNULL _result_stmt) {
-- + #define with_result_set_refs_offSET cql_offsetof(with_result_set_row, name) // count = 1
-- + cql_int32 with_result_set_get_id(with_result_set_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + cql_string_ref _NULLable with_result_set_get_name(with_result_set_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + cql_bool with_result_set_get_rate_is_NULL(with_result_set_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + cql_int64 with_result_set_get_rate_value(with_result_set_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + cql_int32 with_result_set_get_type_value(with_result_set_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + cql_int32 with_result_set_result_count(with_result_set_result_set_ref _NonNULL result_set) {
-- + CQL_WARN_UNUSED cql_code with_result_set_fetch_results(sqlite3 *_NonNULL _db_, with_result_set_result_set_ref _NULLable *_NonNULL result_set) {
-- + IF (_rc_ == SQLITE_OK && !*_result_stmt) _rc_ = cql_no_rows_stmt(_db_, _result_stmt);
CREATE procedure with_result_set()
BEGIN
  SELECT * FROM bar;
END;

-- TEST: grabs values FROM a VIEW that is backed by a table
-- + cql_code select_from_view(sqlite3 *_NonNULL _db_, sqlite3_stmt *_NULLable *_NonNULL _result_stmt) {
-- - .refs_count = 0,
-- - .refs_offSET = 0,
-- + cql_int32 select_from_view_get_id(select_from_view_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + cql_bool select_from_view_get_type_is_NULL(select_from_view_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + cql_int32 select_from_view_get_type_value(select_from_view_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + cql_int32 select_from_view_result_count(select_from_view_result_set_ref _NonNULL result_set) {
-- + CQL_WARN_UNUSED cql_code select_from_view_fetch_results(sqlite3 *_NonNULL _db_, select_from_view_result_set_ref _NULLable *_NonNULL result_set) {
CREATE PROC select_from_view()
BEGIN
  SELECT id, type FROM baz;
END;

-- TEST: CREATE dml FOR a view
-- +  "CREATE VIEW MyView AS "
-- +  "SELECT 1 AS f1, 2 AS f2, 3 AS f3"
CREATE procedure make_view()
BEGIN
   CREATE VIEW MyView AS SELECT 1 AS f1, 2 AS f2, 3 AS f3;
END;

-- TEST: code gen a simple CREATE index statement
-- + "CREATE INDEX index_1 ON bar (id)"
CREATE procedure make_index()
BEGIN
  CREATE index index_1 on bar(id);
END;

-- TEST: CREATE a PROC WITH reader logic WITH more than one arg
-- + cql_code get_data(sqlite3 *_NonNULL _db_, sqlite3_stmt *_NULLable *_NonNULL _result_stmt, cql_string_ref _NonNULL name_, cql_int32 id_) {
CREATE procedure get_data(name_ text NOT NULL, id_ INTEGER NOT NULL)
BEGIN
  SELECT * FROM bar WHERE id = id_ AND name = name_;
END;

-- TEST: CREATE a PROC that uses the new CURSOR fetch strategy
--       then bind values FROM those implicit variables in a CQL statement
--       AND also bind the _has_rows auto local AS well
-- validate auto variable management
-- + #define easy_fetch_C_refs_offSET cql_offsetof(easy_fetch_C_row, name) // count = 1
-- + easy_fetch_C_row C = { ._refs_count_ = 1, ._refs_offset_ = easy_fetch_C_refs_offSET };
-- + sqlite3_stmt *C2_stmt = NULL;
-- note that C2 is never fetched AND therefore has no has_row, we don't want to generate
-- this variable because WITH good warnings it will CREATE an unused variable error
-- - _C2_has_row_
-- + C._has_row_ = _rc_ == SQLITE_ROW;
-- + cql_multifetch(_rc_, C_stmt, 5,
-- +                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &C.id,
-- +                CQL_DATA_TYPE_STRING, &C.name,
-- +                CQL_DATA_TYPE_INT64, &C.rate,
-- +                CQL_DATA_TYPE_INT32, &C.type,
-- +                CQL_DATA_TYPE_DOUBLE, &C.size);
-- + cql_multibind(&_rc_, _db_, &C2_stmt, 2,
-- +               CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_BOOL, C._has_row_,
-- +               CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, C.id);
-- + cql_finalize_stmt(&C_stmt);
-- + cql_teardown_row(C);
-- + cql_finalize_stmt(&C2_stmt);
@attribute(cql:vault_sensitive)
CREATE PROC easy_fetch()
BEGIN
  DECLARE C CURSOR FOR SELECT * FROM bar;
  fetch C;
  call printf("%d %s\n", C.id, C.name);
  DECLARE C2 CURSOR FOR SELECT * FROM bar WHERE C AND id = C.id;
END;

-- TEST: safe NOT NULLable assignment
-- + i2 = i0_NULLable.value;
-- + i2 = 3;
SET i2 := ifNULL(i0_NULLable, 3);

-- TEST: this works too, but the result might be NULLable
-- + do {
-- +  IF (!i0_NULLable.is_NULL) {
-- +    cql_set_notNULL(i0_NULLable, i0_NULLable.value);
-- +    break;
-- +  }
-- +  cql_set_NULLable(i0_NULLable, i1_NULLable.is_NULL, i1_NULLable.value);
-- + } WHILE (0);
SET i0_NULLable := ifNULL(i0_NULLable, i1_NULLable);

-- TEST: CREATE a PROC that takes a NULLable int AND pass it a NULLable int
--       this forces the case WHERE the variable FOR the int has to be
--       reconstituted FROM the .value field
CREATE PROC copy_int(a int, out b int)
BEGIN
  SET b := a;
END;

-- + copy_int(i0_NULLable, &i1_NULLable);
call copy_int(i0_NULLable, i1_NULLable);

-- TEST: try out last_insert_rowid()
-- + cql_set_notNULL(row, sqlite3_last_insert_rowid(_db_));
-- - cql_cleanup
CREATE PROC insert_rowid_reader()
BEGIN
  DECLARE row long INTEGER;
  SET row := last_insert_rowid();
END;

-- TEST: try out changes()
-- + cql_set_notNULL(ct, sqlite3_changes(_db_));
-- - cql_cleanup
CREATE PROC changes_reader()
BEGIN
  DECLARE ct INTEGER;
  SET ct := changes();
END;

-- TEST: try out printf expression
DECLARE s text NOT NULL;
-- + _printf_result = sqlite3_mprintf("%d AND %d", 1, 2);
-- + cql_string_release(s);
-- + s = cql_string_ref_new(_printf_result);
SET s := printf('%d AND %d', 1, 2);

-- + _printf_result = sqlite3_mprintf("%d AND %d", 3, 4)
-- + cql_string_release(s);
-- + s = cql_string_ref_new(_printf_result);
SET s := printf('%d AND %d', 3, 4);

-- TEST: printf inserts casts FOR numeric types (but only AS needed)
-- + sqlite3_mprintf("%lld %lld %lld %llu %d %d %llu %d %f %f %s %f", ((cql_int64)(4)), _tmp_n_int64_%.value,
-- + ((cql_int64)!!(1)), _64(0), ((cql_int32)!!(0)), 0, _64(6), 7, 0.0, 0.0, NULL, ((cql_double)(8)));
SET s := printf('%lld %lld %lld %llu %d %d %llu %d %f %f %s %f', 4, NULLable(5), true, NULL, false, NULL, 6L, 7, 0.0, NULL, NULL, 8);

-- TEST: printf doesn't insert casts when used in SQL
-- + SELECT printf('%lld %lld %lld %llu %d %d %llu %d %f %f %s %f', 5, 5, 1, NULL, 0, NULL, 6, 7, 0.0, NULL, NULL, 8)
SET s := (SELECT printf('%lld %lld %lld %llu %d %d %llu %d %f %f %s %f', 5, NULLable(5), true, NULL, false, NULL, 6L, 7, 0.0, NULL, NULL, 8));

-- TEST: make sure that we use the canonical name FOR 's' in codegen not 'S'.  Even though S is legal.
-- + cql_set_string_ref(&s, _literal%x_);
SET S := 'x';

-- TEST: DECLARE PROC AND call it
-- + /*
-- + DECLARE PROC xyzzy (id INTEGER) (A INTEGER NOT NULL);
-- + */
DECLARE PROC xyzzy(id INTEGER) ( A INTEGER NOT NULL );

-- + _rc_ = xyzzy(_db_, &xyzzy_cursor_stmt, _tmp_n_int_%);
-- +  IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
CREATE PROC xyzzy_test()
BEGIN
  DECLARE xyzzy_cursor CURSOR FOR call xyzzy(1);
END;

-- TEST: DECLARE a simple PROC WITH no dml
-- + /*
-- + DECLARE PROC plugh (id INTEGER);
-- + */
DECLARE PROC plugh(id INTEGER);

-- TEST: CREATE a PROC that returns a mix of possible types
--       in a select
CREATE PROC complex_return()
BEGIN
  SELECT TRUE AS _bool,
   2 AS _integer,
   CAST(3 AS long INTEGER) AS _longint,
   3.0 AS _real,
   'xyz' AS _text,
   CAST(NULL AS bool) AS _NULLable_bool;
END;

-- TEST: CREATE a PROC WITH a nested SELECT within an in statement FOR hierarchical queries
CREATE PROC hierarchical_query(rate_ long INTEGER NOT NULL, limit_ INTEGER NOT NULL, offset_ INTEGER NOT NULL)
BEGIN
  SELECT *
  FROM foo
  WHERE id in (
    SELECT id
    FROM bar
    WHERE rate = rate_
    ORDER BY name
    LIMIT limit_
    offSET offset_
  )
  ORDER BY id;
END;

-- TEST: CREATE a PROC WITH a nested SELECT within a not in statement FOR hierarchical queries
CREATE PROC hierarchical_unmatched_query(rate_ long INTEGER NOT NULL, limit_ INTEGER NOT NULL, offset_ INTEGER NOT NULL)
BEGIN
  SELECT *
  FROM foo
  WHERE id not in (
    SELECT id
    FROM bar
    WHERE rate = rate_
    ORDER BY name
    LIMIT limit_
    offSET offset_
  )
  ORDER BY id;
END;

-- TEST: CREATE a PROC WITH a compound SELECT union form
CREATE PROC union_select()
BEGIN
 SELECT 1 AS A union SELECT 2 AS A;
END;

-- TEST: CREATE a PROC WITH a compound SELECT union all form
CREATE PROC union_all_select()
BEGIN
 SELECT 1 AS A union all SELECT 2 AS A;
END;

-- TEST: CREATE a valid union using NOT NULL columns AND NULLable matching
CREATE PROC union_all_with_NULLable()
BEGIN
  SELECT NULLable('foo') AS name
  union all
  SELECT name FROM bar;
END;

-- TEST: CREATE a simple WITH statement
CREATE PROC with_stmt_using_cursor()
BEGIN
  DECLARE C CURSOR for
    WITH X(a,b,c) AS (SELECT 1,2,3)
    SELECT * FROM X;
  fetch C;
END;

-- TEST: WITH statement top level
CREATE PROC with_stmt()
BEGIN
  WITH X(a,b,c) AS (SELECT 1,2,3) SELECT * FROM X;
END;

-- TEST: WITH recursive statement top level
CREATE PROC with_recursive_stmt()
BEGIN
  WITH recursive X(a,b,c) AS (SELECT 1,2,3 union all SELECT 4,5,6) SELECT * FROM X;
END;

-- TEST: parent procedure
CREATE PROC parent_proc()
BEGIN
  SELECT 1 AS one, 2 AS two, 3 AS three;
END;

-- TEST: child procedure
CREATE PROC parent_proc_child()
BEGIN
  SELECT 4 AS four, 5 AS five, 6 AS six;
END;

-- TEST: fetch NULLable output parameter
-- + _C_has_row_ = _rc_ == SQLITE_ROW;
-- + cql_multifetch(_rc_, C_stmt, 1,
-- +                CQL_DATA_TYPE_INT32, output);
-- + IF (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
-- + *result = _C_has_row_;
CREATE PROC outint_NULLable(out output INTEGER, out result bool NOT NULL)
BEGIN
  DECLARE C CURSOR FOR SELECT 1;
  fetch C into output;
  SET result := C;
END;

-- TEST: fetch NOT NULL output parameter
-- + cql_bool _C_has_row_ = 0;
-- + _C_has_row_ = _rc_ == SQLITE_ROW;
-- + cql_multifetch(_rc_, C_stmt, 1,
-- +                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, output);
-- + IF (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
-- + *result = _C_has_row_;
CREATE PROC outint_notNULL(out output INTEGER NOT NULL, out result bool NOT NULL)
BEGIN
  DECLARE C CURSOR FOR SELECT 1;
  fetch C into output;
  SET result := C;
END;

DECLARE FUNCTION simple_func(int1 INTEGER) INTEGER;

-- TEST: call external function
-- + cql_set_notNULL(_tmp_n_int_%, 2);
-- + result = simple_func(_tmp_n_int_%);
LET result := simple_func(2);

-- TEST: call external function
-- + cql_set_notNULL(_tmp_n_int_2, 1);
-- + _tmp_n_int_1 = simple_func(_tmp_n_int_2);
-- + result = simple_func(_tmp_n_int_1);
SET result := simple_func(simple_func(1));

DECLARE FUNCTION text_func(int1 INTEGER, int2 INTEGER NOT NULL) text NOT NULL;
DECLARE text_result text;

-- TEST: call external text function
-- + cql_set_notNULL(_tmp_n_int_%, 123);
-- + cql_set_string_ref(&_tmp_text_0, text_func(_tmp_n_int_%, 456));
-- + cql_set_string_ref(&text_result, _tmp_text_0);
SET text_result := text_func(123, 456);

-- TEST: CREATE object variable
-- + cql_object_ref obj_var = NULL;
DECLARE obj_var object;

-- TEST: assign NULL to object variable
-- + cql_set_object_ref(&obj_var, NULL);
SET obj_var := NULL;

-- TEST: DECLARE NOT NULL object
-- + cql_object_ref obj_var2 = NULL;
DECLARE obj_var2 object NOT NULL;

DECLARE FUNCTION obj_notNULL_func() object NOT NULL;

-- initialize FOR later use
SET obj_var2 := obj_notNULL_func();

-- TEST: assign var to object variable
-- + cql_set_object_ref(&obj_var, obj_var2);
SET obj_var := obj_var2;

-- remove the NULLability improvement
-- + cql_set_object_ref(&obj_var, NULL);
SET obj_var := NULL;

-- TEST: object comparison
-- +  cql_combine_NULLables(b0_NULLable, !obj_var, !obj_var, obj_var == obj_var);
SET b0_NULLable := obj_var == obj_var;

-- TEST: object variable in IN clause
-- + IF (cql_is_NULLable_true(!obj_var, _tmp_n_object_% == obj_var)) break;
-- + IF (cql_is_NULLable_true(!obj_var, _tmp_n_object_% == obj_var)) break;
SET b0_NULLable := obj_var in (obj_var, obj_var);

-- TEST: object variable in IN clause
-- + IF (_tmp_object_% == obj_var2) break;
SET b2 := obj_var2 in (obj_var2, obj_var2);

-- TEST: object variable in NOT IN clause
-- + IF (cql_is_NULLable_true(!obj_var, _tmp_n_object_% == obj_var)) break;
-- + IF (cql_is_NULLable_true(!obj_var, _tmp_n_object_% == obj_var)) break;
SET b0_NULLable := obj_var not in (obj_var, obj_var);

-- TEST: object variable in NOT IN clause
-- + IF (_tmp_object_% == obj_var2) break;
SET b2 := obj_var2 not in (obj_var2, obj_var2);

-- TEST: PROC WITH object args
-- + void obj_proc(cql_object_ref _NULLable *_NonNULL an_object)
CREATE PROC obj_proc(out an_object object)
BEGIN
  SET an_object := NULL;
END;

-- TEST: CURSOR WITH object in it
-- + cursor_with_object(object_, row);
-- + void cursor_with_object(cql_object_ref _NULLable object_, cursor_with_object_row *_NonNULL _result_)
CREATE PROC cursor_with_object(object_ Object)
BEGIN
  DECLARE C CURSOR LIKE cursor_with_object arguments;
  fetch C FROM arguments;
  out C;
END;

-- TEST: case statement WITH objects
-- + IF (cql_is_NULLable_true(!obj_var, _tmp_n_object_1 == obj_var))
SET i2 := case obj_var when obj_var then 1 else 2 END;

-- TEST: case statement WITH returning objects
-- + cql_set_object_ref(&obj_var, obj_var2);
-- + cql_set_object_ref(&obj_var, NULL);
SET obj_var := case 1 when 1 then obj_var2 else NULL END;

DECLARE FUNCTION obj_func() object;

-- TEST: FUNCTION invocation WITH object function
-- + cql_set_object_ref(&obj_var, obj_func());
SET obj_var := obj_func();

DECLARE FUNCTION obj_func_create() CREATE object;

-- TEST: FUNCTION invocation WITH creater object function
-- +  cql_object_release(obj_var);
-- +  obj_var = obj_func_create();
SET obj_var := obj_func_create();

DECLARE FUNCTION text_func_create() CREATE text;

-- TEST: FUNCTION invocation WITH creater text function
-- cql_string_release(_tmp_n_text_0);
-- _tmp_n_text_0 = text_func_create();
SET text_result := text_func_create();

-- TEST: assign NULLable to object WITH helper OR crash
-- + cql_set_object_ref(&_tmp_n_object_0, obj_func());
-- + cql_invariant(!!_tmp_n_object_0);
-- + cql_set_object_ref(&obj_var2, _tmp_n_object_0);
SET obj_var2 := ifNULL_crash(obj_func());

-- TEST: assign NULLable to object WITH helper OR crash (ifNULL_crash synonym)
-- + cql_set_object_ref(&_tmp_n_object_0, obj_func());
-- + cql_invariant(!!_tmp_n_object_0);
-- + cql_set_object_ref(&obj_var2, _tmp_n_object_0);
SET obj_var2 := ifNULL_crash(obj_func());

-- TEST: assign NULLable to object WITH helper OR throw
-- + cql_set_object_ref(&_tmp_n_object_0, obj_func());
-- + IF (!_tmp_n_object_0) {
-- +   cql_error_trace();
-- +   _rc_ = SQLITE_ERROR;
-- +   goto cql_cleanup;
-- + }
-- + cql_set_object_ref(&obj_var2, _tmp_n_object_0);
SET obj_var2 := ifNULL_throw(obj_func());

-- TEST: assign NULLable to object WITH helper OR crash
-- + cql_object_release(_tmp_n_object_0);
-- + _tmp_n_object_0 = obj_func_create();
-- + cql_invariant(!!_tmp_n_object_0);
-- + cql_set_object_ref(&obj_var2, _tmp_n_object_0);
SET obj_var2 := ifNULL_crash(obj_func_create());

-- TEST: assign NULLable int to an INTEGER
-- + cql_invariant(!i0_NULLable.is_NULL);
-- + i2 = i0_NULLable.value
SET i2 := ifNULL_crash(i0_NULLable);

-- TEST: assign NULLable int to an INTEGER OR throw
-- + IF (i0_NULLable.is_NULL) {
-- +   _rc_ = SQLITE_ERROR;
-- +   goto cql_cleanup;
-- + }
-- + i2 = i0_NULLable.value;
SET i2 := ifNULL_throw(i0_NULLable);

-- TEST: unused temp in unary not emitted
-- - cql_int32 _tmp_int_0 = 0;
-- - cql_int32 _tmp_int_1 = 0;
-- + o = i.value;
-- + o = - 1;
CREATE PROC unused_temp(i INTEGER, out o INTEGER NOT NULL)
BEGIN
  SET o := coalesce(i, -1);
END;

-- TEST: echo something to the output
-- + Garbonzo
-- + chick pea
@echo c, "int Garbonzo; // a chick pea\n";

-- TEST: echo all the escape characters that are supported
-- + //
-- + '%'
@echo c, "//\/'\a\b\f\t\v'\r\n";

-- TEST: echo inside a procedure
-- + void echo_test(void) {
-- + cql_set_string_ref(&s, _literal%before_echo_%);
-- + #define ECHO_TEST 1
-- + cql_set_string_ref(&s, _literal%after_echo_%);
CREATE PROC echo_test()
BEGIN
  DECLARE s text;
  SET s := "before echo";
  @echo c, "#define ECHO_TEST 1\n";
  SET s := "after echo";
END;

-- TEST: insert OR replace form
-- +  "INSERT OR REPLACE INTO bar(id, type) VALUES(1, 5)"
insert OR replace into bar(id, type) values (1,5);

-- TEST: insert default from
-- +  "INSERT INTO foo DEFAULT VALUES"
INSERT INTO foo default values;

-- TEST: insert FROM stored procedure
-- + "INSERT INTO bar(id, type) VALUES(?, ?)"
-- + cql_code insert_VALUES(sqlite3 *_NonNULL _db_, cql_int32 id_, cql_NULLable_int32 type_) {
CREATE PROC insert_VALUES(id_ INTEGER NOT NULL, type_ INTEGER)
BEGIN
  INSERT INTO bar(id, type) values (id_, type_);
END;

-- TEST: alter TABLE add column
-- +   _rc_ = cql_exec(_db_,
-- + "ALTER TABLE bar ADD COLUMN size REAL"
CREATE PROC alter_table_test()
BEGIN
  alter TABLE bar add column size real;
END;

-- TEST: drop table
-- + _rc_ = cql_exec(_db_,
-- + "DROP TABLE IF EXISTS bar"
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
-- + _rc_ = SQLITE_OK;
CREATE PROC drop_table_test()
BEGIN
  drop TABLE IF exists bar;
END;

-- TEST: use a procedure to get a result set
-- + cql_code uses_proc_for_result(sqlite3 *_NonNULL _db_, sqlite3_stmt *_NULLable *_NonNULL _result_stmt)
-- + *_result_stmt = NULL;
-- + _rc_ = with_result_set(_db_, _result_stmt);
-- +1 cql_finalize_stmt(_result_stmt);
CREATE procedure uses_proc_for_result()
BEGIN
  call with_result_set();
END;

-- TEST: DECLARE a void func
DECLARE FUNCTION voidfunc() INTEGER;

-- TEST: use a SELECT exists clause
-- + "SELECT EXISTS (SELECT * "
-- + "FROM bar)"
SET b2 := (SELECT EXISTS(SELECT * FROM bar));

-- TEST: FOR expand of SELECT * columns FROM whole result
-- + _rc_ = cql_prepare(_db_, &expanded_select_stmt,
-- + "SELECT id, name, rate, type, size "
-- + "FROM bar"
DECLARE expanded_SELECT CURSOR FOR SELECT * FROM bar;

-- TEST: FOR expand of SELECT * columns FROM table
-- + "SELECT bar.id, bar.name, bar.rate, bar.type, bar.size "
-- + "FROM bar"
DECLARE table_expanded_SELECT CURSOR FOR SELECT bar.* FROM bar;

-- TEST: use a long literal
-- + l2 = _64(3147483647);
SET l2 := 3147483647L;

-- TEST: use a long literal
-- + l2 = _64(3147483647);
SET l2 := 3147483647;

-- TEST: use drop index in a proc
-- + "DROP INDEX index_1"
CREATE PROC index_dropper()
BEGIN
  drop index index_1;
END;

-- TEST: simple DML statements FOR json_schema cg
-- +2 "INSERT INTO foo(id) VALUES(NULL)"
-- + "UPDATE bar "
-- + "SET name = 'bar' "
-- + "DELETE FROM foo WHERE id = 1"
CREATE PROC misc_dml_proc()
BEGIN
  INSERT INTO foo VALUES(NULL);
  INSERT INTO foo(id) VALUES(NULL);
  UPDATE bar SET name = 'bar' WHERE name = 'baz';
  DELETE FROM foo WHERE id = 1;
END;

-- TEST: use dummy data
-- + INSERT INTO bar(id, name, rate, type, size) VALUES(_seed_, printf('name_%d', _seed_), _seed_, _seed_, _seed_)
-- + @DUMMY_SEED(123) @DUMMY_DEFAULTS @DUMMY_NULLABLES;
-- + _seed_ = 123;
-- + "INSERT INTO bar(id, name, rate, type, size) VALUES(?, printf('name_%d', ?), ?, ?, ?)"
-- + cql_multibind(&_rc_, _db_, &_temp_stmt, 5,
-- +4              CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, _seed_,
-- +               CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, _seed_);
CREATE PROC dummy_user()
BEGIN
  INSERT INTO bar () values () @dummy_seed(123) @dummy_NULLables @dummy_defaults;
END;

CREATE PROC proc_with_out_arg(out foo text)
BEGIN
  SET foo := 'x';
END;

-- TEST: the incoming arg must be NULLed
--  when we call proc_with_out_arg we have to release the out arg before we call it OR leak
-- (note: run tests verify this too)
-- bar is local AND it is forced to NULL to start
-- + cql_string_ref bar = NULL;
-- foo is the out arg, we clobber it to a safe value
-- + *(void **)foo = NULL;
-- foo is SET to something useful
-- + cql_set_string_ref(&*foo, _literal_%x_%);
-- we have to release the something useful before we make the call
-- + cql_set_string_ref(&*foo, NULL);
-- + proc_with_out_arg(foo);
-- we have to release bar before we make the call
-- + cql_set_string_ref(&bar, NULL);
-- + proc_with_out_arg(&bar);
CREATE PROC calls_out_proc(out foo text)
BEGIN
  SET foo := 'x';
  DECLARE bar text;
  call proc_with_out_arg(foo);
  call proc_with_out_arg(bar);
END;

-- TEST: CREATE blob variable
-- + cql_blob_ref blob_var = NULL;
DECLARE blob_var blob;

-- TEST: CREATE blob variable2
-- + cql_blob_ref blob_var2 = NULL;
DECLARE blob_var2 blob NOT NULL;

DECLARE FUNCTION blob_notNULL_func() blob NOT NULL;

-- initialize FOR later use
SET blob_var2 := blob_notNULL_func();

-- TEST: assign NULL to blob variable
-- + cql_set_blob_ref(&blob_var, NULL);
SET blob_var := NULL;

-- TEST: assign var to blob variable
-- + cql_set_blob_ref(&blob_var, blob_var2);
SET blob_var := blob_var2;

-- remove the NULLability improvement
-- + cql_set_blob_ref(&blob_var, NULL);
SET blob_var := NULL;

-- TEST: blob comparison "=="
-- + cql_combine_NULLables(b0_NULLable, !blob_var, !blob_var, cql_blob_equal(blob_var, blob_var));
SET b0_NULLable := blob_var == blob_var;

-- TEST: blob comparison "IS" NULL
-- + cql_set_notNULL(b0_NULLable, !blob_var);
SET b0_NULLable := blob_var IS NULL;

-- TEST: blob comparison "!="
-- + cql_combine_NULLables(b0_NULLable, !blob_var, !blob_var, !cql_blob_equal(blob_var, blob_var));
SET b0_NULLable := blob_var != blob_var;

-- TEST: blob comparison "<>"
-- + cql_combine_NULLables(b0_NULLable, !blob_var, !blob_var, !cql_blob_equal(blob_var, blob_var));
SET b0_NULLable := blob_var <> blob_var;

-- TEST: blob comparison "IS"
-- + cql_set_notNULL(b0_NULLable, cql_blob_equal(blob_var, blob_var));
SET b0_NULLable := blob_var IS blob_var;

-- TEST: blob comparison "IS NOT"
-- + cql_set_notNULL(b0_NULLable, !cql_blob_equal(blob_var, blob_var));
SET b0_NULLable := blob_var IS NOT blob_var;

-- TEST: blob variable in IN clause
-- + cql_set_notNULL(_tmp_n_bool_0, 1);
-- + IF (cql_blob_equal(_tmp_n_blob_%, blob_var)) break;
-- + IF (cql_blob_equal(_tmp_n_blob_%, blob_var)) break;
-- + cql_set_notNULL(_tmp_n_bool_0, 0);
SET b0_NULLable := blob_var in (blob_var, blob_var);

-- TEST: blob variable in IN clause
-- + _tmp_bool_0 = 1;
-- + IF (cql_blob_equal(_tmp_blob_%, blob_var)) break;
-- + IF (cql_blob_equal(_tmp_blob_%, blob_var2)) break;
-- + _tmp_bool_0 = 0;
SET b2 := blob_var2 in (blob_var, blob_var2);

-- TEST: blob variable in NOT IN clause
-- + cql_set_notNULL(_tmp_n_bool_0, 0);
-- + IF (cql_blob_equal(_tmp_n_blob_%, blob_var)) break;
-- + IF (cql_blob_equal(_tmp_n_blob_%, blob_var)) break;
-- + cql_set_notNULL(_tmp_n_bool_0, 1);
SET b0_NULLable := blob_var not in (blob_var, blob_var);

-- TEST: blob variable in NOT IN clause
-- + IF (cql_blob_equal(_tmp_blob_%, blob_var)) break;
-- + IF (cql_blob_equal(_tmp_blob_%, blob_var2)) break;
-- + b2 = _tmp_bool_0;
SET b2 := blob_var2 not in (blob_var, blob_var2);

-- TEST: PROC WITH blob args
-- + void blob_proc(cql_blob_ref _NULLable *_NonNULL a_blob)
CREATE PROC blob_proc(out a_blob blob)
BEGIN
  SET a_blob := NULL;
END;

-- TEST: case statement WITH blobs
-- + IF (cql_is_NULLable_true(!blob_var, _tmp_n_blob_1 == blob_var))
SET i2 := case blob_var when blob_var then 1 else 2 END;

-- TEST: case statement WITH returning blobs
-- + cql_set_blob_ref(&blob_var, blob_var2);
-- + cql_set_blob_ref(&blob_var, NULL);
SET blob_var := case 1 when 1 then blob_var2 else NULL END;

DECLARE FUNCTION blob_func() blob;

-- TEST: FUNCTION invocation WITH blob function
-- + cql_set_blob_ref(&blob_var, blob_func());
SET blob_var := blob_func();

DECLARE FUNCTION blob_func_create() CREATE blob;

-- TEST: FUNCTION invocation WITH creater blob function
-- +  cql_blob_release(blob_var);
-- +  blob_var = blob_func_create();
SET blob_var := blob_func_create();

-- make a TABLE WITH blobs in it
CREATE TABLE blob_table (
  blob_id INTEGER NOT NULL,
  b_notNULL blob NOT NULL,
  b_NULLable blob
);

-- TEST: fetch a NULLable blob
-- + cql_column_NULLable_blob_ref(_temp_stmt, 0, &blob_var);
SET blob_var := (SELECT b_NULLable FROM blob_table WHERE blob_id = 1);

-- TEST: fetch a NOT NULL blob
-- + cql_column_blob_ref(_temp_stmt, 0, &_tmp_blob_0);
SET blob_var := (SELECT b_notNULL FROM blob_table WHERE blob_id = 1);

-- some NOT NULL blob object we can use
DECLARE blob_var_notNULL blob NOT NULL;

-- initialize FOR later use
SET blob_var_notNULL := blob_notNULL_func();

-- TEST: bind a NULLable blob AND a NOT NULL blob
-- + INSERT INTO blob_table(blob_id, b_NULLable, b_notNULL) VALUES(0, blob_var, blob_var_notNULL);
-- + "INSERT INTO blob_table(blob_id, b_NULLable, b_notNULL) VALUES(0, ?, ?)"
-- + cql_multibind(&_rc_, _db_, &_temp_stmt, 2,
-- +               CQL_DATA_TYPE_BLOB, blob_var,
-- +               CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_BLOB, blob_var_notNULL);
INSERT INTO blob_table(blob_id, b_NULLable, b_notNULL) VALUES(0, blob_var, blob_var_notNULL);

-- TEST: a result SET that includes blobs
CREATE PROC blob_returner()
BEGIN
  SELECT * FROM blob_table;
END;

-- TEST: forcing NULL SET of object temporary by having no else case
-- + cql_set_object_ref(&obj_var, NULL);
SET obj_var := case when 1 then obj_var END;

-- TEST: force a PROC WITH no arg list
-- + void voidproc(void) {
CREATE PROC voidproc()
BEGIN
 DECLARE unused int;
END;

-- TEST: CREATE an output struct proc
-- + #define out_cursor_proc_C_refs_offSET cql_offsetof(out_cursor_proc_C_row, name) // count = 3
-- + memset(_result_, 0, sizeof(*_result_));
-- + out_cursor_proc_C_row C = { ._refs_count_ = 3, ._refs_offset_ = out_cursor_proc_C_refs_offSET };
-- + _result_->_has_row_ = C._has_row_;
-- + _result_->id = C.id;
-- + cql_set_string_ref(&_result_->name, C.name);
-- + _result_->rate = C.rate;
-- + _result_->type = C.type;
-- + _result_->size = C.size;
-- + cql_set_string_ref(&_result_->extra1, C.extra1);
-- + cql_set_string_ref(&_result_->extra2, C.extra2);
-- + DECLARE PROC out_cursor_proc () OUT (id INTEGER NOT NULL, name TEXT, rate LONG_INT, type INTEGER, size REAL, extra1 TEXT NOT NULL, extra2 TEXT NOT NULL) USING TRANSACTION;
CREATE PROC out_cursor_proc()
BEGIN
  DECLARE C CURSOR FOR SELECT bar.*, 'xyzzy' extra1, 'plugh' extra2 FROM bar;
  fetch C;
  out C;
END;

-- TEST: fetch FROM an output struct proc
-- + #define read_cursor_proc_C_refs_offSET cql_offsetof(read_cursor_proc_C_row, name) // count = 3
-- + read_cursor_proc_C_row C = { ._refs_count_ = 3, ._refs_offset_ = read_cursor_proc_C_refs_offSET };
-- +2 cql_teardown_row(C);
-- +1 _rc_ = out_cursor_proc(_db_, (out_cursor_proc_row *)&C);
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
CREATE PROC read_cursor_proc()
BEGIN
  DECLARE C CURSOR fetch FROM call out_cursor_proc();
END;

-- TEST: DECLARE a CURSOR AND do a fetch AS separate actions
-- +1  declare_cursor_then_fetch_from_proc_C_row C = { ._refs_count_ = 3, ._refs_offset_ = declare_cursor_then_fetch_from_proc_C_refs_offSET };
-- +2  cql_teardown_row(C);
-- +1  _rc_ = out_cursor_proc(_db_, (out_cursor_proc_row *)&C);
CREATE PROC declare_cursor_then_fetch_from_proc()
BEGIN
  DECLARE C CURSOR LIKE out_cursor_proc;
  fetch C FROM call out_cursor_proc();
END;

-- TEST: PROC decl WITH out args
-- + DECLARE PROC fetcher_proc () OUT (a INTEGER, b TEXT);
DECLARE PROC fetcher_proc() out (a INTEGER, b text);

-- TEST: All void all day
-- + DECLARE PROC totally_void_proc ();
DECLARE PROC totally_void_proc();

-- TEST: call out PROC LIKE a function
-- + SET i2 := outparm_test();
-- + _rc_ = outparm_test(_db_, &i2);
-- +  IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
SET i2 := outparm_test();

DECLARE PROC compute(in a_ INTEGER NOT NULL, out b_ INTEGER NOT NULL);

-- TEST: call out PROC LIKE a function, this one has args
-- + compute(1, &_tmp_int_1);
-- + compute(_tmp_int_1, &i2);
SET i2 := compute(compute(1));

-- a dml method
DECLARE PROC dml_compute(in a_ INTEGER NOT NULL, out b_ INTEGER NOT NULL) USING TRANSACTION;

-- TEST: call out PROC LIKE a function, this one has args AND uses the db
-- + _rc_ = dml_compute(_db_, 1, &_tmp_int_1);
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
-- + _rc_ = dml_compute(_db_, _tmp_int_1, &i2);
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
SET i2 := dml_compute(dml_compute(1));

-- TEST: write the result of a proc-as-func call to an out variable
-- + _rc_ = dml_compute(_db_, 1, &*a_);
CREATE PROC dml_user(out a_ INTEGER NOT NULL)
BEGIN
  SET a_ := dml_compute(1);
END;

-- a test TABLE FOR the following case
CREATE TABLE threads (
 thread_key long int NOT NULL
);

-- TEST: nested subquery in a proc
-- this forces the json_schema runtime to run over an atypical table_factor
-- + _rc_ = cql_prepare(_db_, _result_stmt,
-- + "SELECT thread_key "
-- + "FROM (SELECT thread_key "
-- + "FROM threads) AS T"
CREATE procedure thread_theme_info_list(thread_key_ LONG INT NOT NULL)
BEGIN
  SELECT *
  FROM (SELECT thread_key FROM threads) T;
END;

-- TEST: value CURSOR fetch
-- + _seed_ = 123;
-- + C._has_row_ = 1;
-- + C.id = _seed_;
-- + char *_printf_result = sqlite3_mprintf("name_%d", _seed_);
-- + cql_string_release(_tmp_text_0);
-- + _tmp_text_0 = cql_string_ref_new(_printf_result);
-- + sqlite3_free(_printf_result);
-- + cql_set_string_ref(&C.name, _tmp_text_0);
-- + cql_set_notNULL(C.rate, _seed_);
-- + cql_set_notNULL(C.type, _seed_);
-- + cql_set_notNULL(C.size, _seed_);
-- This should not be a dml proc, it doesn't actually use the db
-- + fetch_values_dummy(void)
-- - _rc_
-- - cql_cleanup
CREATE PROC fetch_values_dummy()
BEGIN
  DECLARE C CURSOR LIKE SELECT * FROM bar;
  fetch C() FROM VALUES() @dummy_seed(123) @dummy_NULLables;
END;

-- TEST: value CURSOR fetch, using type syntax
-- this CURSOR has the fields of bar plus xx AND yy
-- + cql_int32 id;
-- + cql_NULLable_int64 rate;
-- + cql_NULLable_int32 type;
-- + cql_NULLable_double size;
-- + cql_NULLable_double xx;
-- + cql_string_ref _NULLable name;
-- + cql_string_ref _NULLable yy;
-- + fetch_values_extended_C_row C = { ._refs_count_ = 2, ._refs_offset_ = fetch_values_extended_C_refs_offSET };
-- + cql_set_string_ref(&C.name, _tmp_text_0);
-- + cql_set_notNULL(C.rate, _seed_);
-- + cql_set_notNULL(C.type, _seed_);
-- + cql_set_notNULL(C.size, _seed_);
-- + cql_set_notNULL(C.xx, _seed_);
-- + fetch_values_extended(void)

-- CREATE PROC fetch_values_extended()
-- BEGIN
--   DECLARE C CURSOR LIKE (like bar, xx real, yy text);
--   fetch C() FROM VALUES() @dummy_seed(123) @dummy_NULLables;
-- END;

-- TEST: c style literal
-- + SET x := "\"Testing\" \\''";
CREATE PROC c_literal(out x text)
BEGIN
  SET x := "\"Testing\" \\''";
END;

-- TEST: no cleanup label needed proc
-- - cql_cleanup
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto catch_start%; }
-- + catch_start%:
CREATE PROC no_cleanup_label_needed_proc()
BEGIN
  BEGIN try
    DECLARE C CURSOR FOR SELECT 1 AS N;
    fetch C;
  end try;
  BEGIN catch
    DECLARE x INTEGER;
  end catch;
END;

-- TEST: no code after the last label
-- BEGIN try AND BEGIN catch implyl dml proc
-- + cql_code no_code_after_catch(sqlite3 *_NonNULL _db_)
CREATE PROC no_code_after_catch()
BEGIN
  BEGIN try
    @attribute(foo) -- just messing WITH the tree
    DECLARE x INTEGER;
  end try;
  BEGIN catch
    @attribute(bar) -- just messing WITH the tree
    DECLARE y INTEGER;
  end catch;
END;

-- TEST: void CURSOR fetcher
-- + void out_no_db(out_no_db_row *_NonNULL _result_) {
-- + memset(_result_, 0, sizeof(*_result_));
-- + out_no_db_C_row C = { 0 };
-- + C._has_row_ = 1;
-- + C.A = 3;
-- + C.B = 12;
-- + _result_->_has_row_ = C._has_row_;
-- + _result_->A = C.A;
-- + _result_->B = C.B;
-- + DECLARE PROC out_no_db () OUT (A INTEGER NOT NULL, B REAL NOT NULL);
CREATE PROC out_no_db()
BEGIN
  DECLARE C CURSOR LIKE SELECT 1 A, 2.5 B;
  fetch C(A,B) FROM VALUES(3,12);
  out C;
END;

-- TEST: DECLARE CURSOR LIKE cursor
-- + declare_cursor_like_cursor_C0_row C0 = { 0 };
-- + declare_cursor_like_cursor_C1_row C1 = { 0 };
-- + memset(_result_, 0, sizeof(*_result_));
-- + C1._has_row_ = 1;
-- + C1.A = 3;
-- + C1.B = 12;
-- + _result_->_has_row_ = C1._has_row_;
-- + _result_->A = C1.A;
-- + _result_->B = C1.B;
CREATE PROC declare_cursor_like_cursor()
BEGIN
  DECLARE C0 CURSOR LIKE SELECT 1 A, 2.5 B;
  DECLARE C1 CURSOR LIKE C0;
  fetch C1(A,B) FROM VALUES(3,12);
  out C1;
END;

-- TEST: DECLARE CURSOR LIKE proc
-- + void declare_cursor_like_proc(declare_cursor_like_proc_row *_NonNULL _result_) {
-- + memset(_result_, 0, sizeof(*_result_));
-- + _result_->_has_row_ = C._has_row_;
-- + _result_->_refs_count_ = 1;
-- + _result_->_refs_offset_ = declare_cursor_like_proc_refs_offset;
-- + _result_->a = C.a;
-- + cql_set_string_ref(&_result_->b, C.b);
-- + cql_teardown_row(C);
-- - Error
CREATE PROC declare_cursor_like_proc()
BEGIN
  DECLARE C CURSOR LIKE fetcher_proc;
  out C;
END;

-- TEST: DECLARE a CURSOR LIKE a table
-- + void declare_cursor_like_table(declare_cursor_like_table_row *_NonNULL _result_) {
-- + declare_cursor_like_table_C_row C = { ._refs_count_ = 1, ._refs_offset_ = declare_cursor_like_table_C_refs_offSET };
-- + memset(_result_, 0, sizeof(*_result_));
-- + _result_->_has_row_ = C._has_row_;
-- + _result_->_refs_offset_ = declare_cursor_like_table_refs_offset;
-- + _result_->id = C.id;
-- + cql_set_string_ref(&_result_->name, C.name);
-- + _result_->rate = C.rate;
-- + _result_->type = C.type;
-- + _result_->size = C.size;
-- + cql_teardown_row(C);
-- - Error
CREATE PROC declare_cursor_like_table()
BEGIN
  DECLARE C CURSOR LIKE bar;
  out C;
END;

-- TEST: DECLARE a CURSOR LIKE a view
-- + void declare_cursor_like_view_fetch_results( declare_cursor_like_view_result_set_ref _NULLable *_NonNULL result_set) {
-- + declare_cursor_like_view_C_row C = { 0 };
-- + _result_->_has_row_ = C._has_row_;
-- + _result_->f1 = C.f1;
-- + _result_->f2 = C.f2;
-- + _result_->f3 = C.f3;
-- - Error
CREATE PROC declare_cursor_like_view()
BEGIN
  DECLARE C CURSOR LIKE MyView;
  out C;
END;

-- TEST: stress case FOR quote management
-- the backslash must be preserved in a regular sql string AND then escaped
-- the newlines in the c string are turned into newline characters in the SQL string
-- but they have to be escaped due to being embedded in a c string
-- the ones WITH a leading space are the echoed sql, the strings are not C escaped there
-- so this checks both paths
-- +  DELETE FROM bar WHERE name LIKE "\n\n";
-- + "DELETE FROM bar WHERE name LIKE '\n\n'"
-- +  DELETE FROM bar WHERE name = ' '' \n '' \';
-- + "DELETE FROM bar WHERE name = ' '' \\n '' \\'"
-- +  DELETE FROM bar WHERE name <> "'";
-- + "DELETE FROM bar WHERE name <> ''''"
-- +  DELETE FROM bar WHERE name >= '\';
-- + "DELETE FROM bar WHERE name >= '\\'"
CREATE PROC weird_quoting()
BEGIN
  DELETE FROM bar WHERE name LIKE "\n\n";
  -- the newline looking thing is NOT an escape sequence it's a pain in the ass...
  -- DELETE FROM bar WHERE name = ' '' \n '' \';
  -- lots of transforms required to get this right
  DELETE FROM bar WHERE name != "\'";
  -- another tricky case
  DELETE FROM bar WHERE name >= '\';
END;

-- TEST: CREATE a TABLE WITH a long INTEGER autoinc column
-- this requires the workaround of downgradeing the long to int
-- note: sqlite INTEGERs can hold 64 bits so they are already "long"
-- + id LONG_INT PRIMARY KEY AUTOINCREMENT,
-- + "id INTEGER PRIMARY KEY AUTOINCREMENT, "
CREATE PROC long_auto_table_maker()
BEGIN
  CREATE TABLE long_int_autoinc (
    id long primary key autoincrement,
    name text
  );
END;

DECLARE PROC blob_out(out x blob);

-- TEST: force a blob variable to be cleared to NULL before a PROC cll
-- call on out parameter.  This is important because the blob
-- might be holding a value AND the out arg is assume to be junk
-- +   cql_blob_ref b = NULL;
-- +   cql_set_blob_ref(&b, NULL);
-- +   blob_out(&b);
-- +   cql_blob_release(b);
CREATE PROC blob_call1()
BEGIN
 DECLARE b blob;
 call blob_out(b);
END;

-- TEST: force a blob variable to be cleared to NULL before a function
-- call on out parameter.  This is important because the blob
-- might be holding a value AND the out arg is assume to be junk
-- +   cql_blob_ref b = NULL;
-- +   cql_set_blob_ref(&b, NULL);
-- +   blob_out(&b);
-- +   cql_blob_release(b);
CREATE PROC blob_call2()
BEGIN
 DECLARE b blob;
 SET b := blob_out(); -- use FUNCTION call syntax should be the same
END;

-- TEST: forces us to SET a blob to NULL via else.  This is not the store code path
-- + cql_set_blob_ref(&b, b1);
-- + cql_set_blob_ref(&b, NULL);
CREATE PROC blob_no_else()
BEGIN
  DECLARE b blob;
  DECLARE b1 blob;
  SET b := case b when b then b1 END;
END;

-- TEST: use with-insert form
-- +  _rc_ = cql_exec(_db_,
-- + "WITH "
-- + "x (a) AS (SELECT 111) "
-- + "INSERT INTO foo(id) VALUES(ifNULL(( SELECT a "
-- + "FROM x ), 0))");
WITH x(a) AS (SELECT 111)
INSERT INTO foo values ( ifNULL((SELECT a FROM x), 0));

-- TEST: use insert FROM SELECT (put this in a PROC to force the schema utils to walk it)
-- + "WITH "
-- + "x (a) AS (SELECT 111) "
-- + "INSERT INTO foo(id) SELECT a "
-- + "FROM x"
CREATE PROC with_inserter()
BEGIN
  WITH x(a) AS (SELECT 111)
    INSERT INTO foo SELECT * FROM x;
END;

DECLARE SELECT func SqlUserFunc(id INTEGER) real NOT NULL;

-- TEST: invoke a declared user function
-- + _rc_ = cql_prepare(_db_, &_temp_stmt,
-- +  "SELECT SqlUserFunc(123)"
SET r2 := (SELECT SqlUserFunc(123));

-- TEST: multiple rewrites complex arg filter
--
-- Note: we're doing something that's legal but not really very useful here just to force the codegen.
-- the out_arg should still be present in the args (and we check it here plus the code is required to compile)
-- AND we have to be able to correctly code gen two different LIKE args cases in different locations.
-- It's hard to think of a real use case FOR this but I want to make sure the rewriter doesn't screw it up.
--
-- + cql_code multi_rewrite(sqlite3 *_NonNULL _db_,
-- + cql_int32 blob_id_, cql_blob_ref _NonNULL b_notNULL_, cql_blob_ref _NULLable b_NULLable_,
-- + cql_int32 id_, cql_string_ref _NULLable name_, cql_NULLable_int64 rate_, cql_NULLable_int32 type_,
-- + cql_NULLable_double size_, cql_int32 *_NonNULL out_arg)
-- + "INSERT INTO blob_table(blob_id, b_notNULL, b_NULLable) VALUES(?, ?, ?)"
-- + cql_multibind(&_rc_, _db_, &_temp_stmt, 3,
-- +               CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, blob_id_,
-- +               CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_BLOB, b_notNULL_,
-- +               CQL_DATA_TYPE_BLOB, b_NULLable_);
CREATE PROC multi_rewrite(like blob_table, LIKE bar, out out_arg INTEGER NOT NULL)
BEGIN
  INSERT INTO blob_table FROM arguments;
  SET out_arg := 1;
END;

-- TEST: fetch to a CURSOR FROM another cursor
-- + C1._has_row_ = 1;
-- + C1.A = C0.A;
-- + cql_set_string_ref(&C1.B, C0.B);
CREATE PROC fetch_to_cursor_from_cursor()
BEGIN
  DECLARE C0 CURSOR LIKE SELECT 1 A, "foo" B;
  DECLARE C1 CURSOR LIKE C0;
  fetch C0 FROM values (2, "bar");
  fetch C1 FROM C0;
  out C1;
END;

-- TEST loop statement CURSOR WITH autofetch
-- + sqlite3_stmt *C_stmt = NULL;
-- + loop_statement_cursor_C_row C = { 0 };
-- + "SELECT 1"
-- + C._has_row_ = _rc_ == SQLITE_ROW;
-- + IF (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
-- + IF (!C._has_row_) break
CREATE PROC loop_statement_cursor()
BEGIN
  DECLARE C CURSOR FOR SELECT 1 A;
  loop fetch C
  BEGIN
    call printf("%d\n", C.A);
  END;
END;

-- TEST loop statement CURSOR WITH autofetch
-- + sqlite3_stmt *C_stmt = NULL;
-- + cql_bool _C_has_row_ = 0;
-- + cql_int32 A_ = 0;
-- + "SELECT 1"
-- + _C_has_row_ = _rc_ == SQLITE_ROW;
-- + IF (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
-- + IF (!_C_has_row_) break
CREATE PROC loop_statement_not_auto_cursor()
BEGIN
  DECLARE C CURSOR FOR SELECT 1 A;
  DECLARE A_ INTEGER NOT NULL;
  loop fetch C into A_
  BEGIN
    call printf("%d\n", A_);
  END;
END;

@attribute(cql:suppress_result_set)
CREATE PROC simple_select()
BEGIN
  SELECT 1 x;
END;

-- TEST: call FOR CURSOR in loop
-- one release in cleanup; one in the loop
-- + IF (!(i.value < 5)) break;
-- +2 cql_finalize_stmt(&C_stmt);
-- + _rc_ = simple_select(_db_, &C_stmt);
CREATE PROC call_in_loop()
BEGIN
  DECLARE i INTEGER;
  SET i := 0;
  WHILE i < 5
  BEGIN
     SET i := i + 1;
     DECLARE C CURSOR FOR call simple_select();
     fetch C;
  END;
END;

-- TEST: same, but WITH a NULLable condition
-- + IF (!cql_is_NULLable_true(_tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value)) break;
-- +2 cql_finalize_stmt(&C_stmt);
-- + _rc_ = simple_select(_db_, &C_stmt);
CREATE PROC call_in_loop_with_NULLable_condition()
BEGIN
  DECLARE i int;
  SET i := NULLable(0);
  WHILE i < 5
  BEGIN
    SET i := i + 1;
    DECLARE C CURSOR FOR call simple_select();
    fetch C;
  END;
END;

-- TEST: call in loop WITH boxing
-- one release in cleanup, one before the fetch
-- +2 cql_object_release(C_object_);
-- one release in cleanup
-- +1 cql_object_release(D_object_);
-- +1 cql_object_release(box);
-- + C_object_ = cql_box_stmt(C_stmt);
-- + cql_set_object_ref(&box, C_object_);
-- + D_stmt = cql_unbox_stmt(D_object_);
-- + C_object_ = cql_box_stmt(C_stmt);
CREATE PROC call_in_loop_boxed()
BEGIN
  DECLARE i INTEGER;
  SET i := 0;
  WHILE i < 5
  BEGIN
     SET i := i + 1;
     DECLARE C CURSOR FOR call simple_select();
     DECLARE box object<C cursor>;
     SET box FROM CURSOR C;
     DECLARE D CURSOR FOR box;
     fetch D;
  END;
END;

-- TEST: verify the decl, this is only FOR later tests
-- + DECLARE PROC out_union_helper () OUT UNION (x INTEGER NOT NULL);
CREATE PROC out_union_helper()
BEGIN
  DECLARE C CURSOR LIKE SELECT 1 x;
  fetch C using 1 x;
  out union C;
END;

-- TEST: verify the decl, this is only FOR later tests
-- + DECLARE PROC out_union_dml_helper () OUT UNION (x INTEGER NOT NULL) USING TRANSACTION;
CREATE PROC out_union_dml_helper()
BEGIN
  DECLARE C CURSOR FOR SELECT 1 x;
  fetch C;
  out union C;
END;

-- TEST: call out union in a loop
-- two instances, one FOR the call AND one at cleanup
-- +2 cql_object_release(C_result_set_);
-- + out_union_helper_fetch_results(&C_result_set_);
-- + C_row_num_ = C_row_count_ = -1;
-- + C_row_count_ = cql_result_set_get_count((cql_result_set_ref)C_result_set_);
-- + C_row_num_++;
-- + C._has_row_ = C_row_num_ < C_row_count_;
-- + cql_copyoutrow(NULL, (cql_result_set_ref)C_result_set_, C_row_num_, 1,
CREATE PROC call_out_union_in_loop()
BEGIN
  DECLARE i INTEGER;
  SET i := 0;
  WHILE i < 5
  BEGIN
     SET i := i + 1;
     DECLARE C CURSOR FOR call out_union_helper();
     fetch C;
  END;
END;

-- TEST: here we CREATE a PROC that is going to forward the result of out union AS its own result
-- + DECLARE PROC forward_out_union () OUT UNION (x INTEGER NOT NULL)
-- - USING TRANSACTION
-- + *_result_set_ = NULL;
-- + out_union_helper_fetch_results((out_union_helper_result_set_ref *)_result_set_);
-- + IF (!*_result_set_) *_result_set_ = (forward_out_union_result_set_ref)cql_no_rows_result_set();
-- +1 cql_object_release(*_result_set_);
CREATE PROC forward_out_union()
BEGIN
  call out_union_helper();
END;

-- DECLARE one, this ensures we have the necessary types after the decl (the row type esp.)
DECLARE PROC extern_out_union_helper () OUT UNION (x INTEGER NOT NULL);

-- TEST: this should still compile even though the body of the PROC isn't here
-- + extern_out_union_helper_fetch_results((extern_out_union_helper_result_set_ref *)_result_set_);
-- +1 cql_object_release(*_result_set_);
CREATE PROC forward_out_union_extern()
BEGIN
  call extern_out_union_helper();
END;

-- TEST: forward out union result, WITH dml proc
-- + DECLARE PROC forward_out_union_dml () OUT UNION (x INTEGER NOT NULL) USING TRANSACTION;
-- + *_result_set_ = NULL;
-- +  _rc_ = out_union_dml_helper_fetch_results(_db_, (out_union_dml_helper_result_set_ref *)_result_set_);
-- +1 cql_object_release(*_result_set_);
CREATE PROC forward_out_union_dml()
BEGIN
  call out_union_dml_helper();
END;

-- TEST: ensure cursors work outside of a proc
--  _rc_ = cql_prepare(_db_, &global_cursor_stmt,
--    "SELECT 1 AS a, 2 AS b"
DECLARE global_cursor CURSOR FOR SELECT 1 a, 2 b;

-- TEST: fetch FROM global cursor
-- + _rc_ = sqlite3_step(global_cursor_stmt);
-- + global_cursor._has_row_ = _rc_ == SQLITE_ROW;
fetch global_cursor;

-- TEST: use LIKE in an expression
-- +  i2 = cql_string_like(_literal_%_x_, _literal_%_y_) == 0;
SET i2 := 'x' LIKE 'y';

-- TEST: use not LIKE in an expression
-- +  i2 = cql_string_like(_literal_%_x_, _literal_%_y_) != 0;
SET i2 := 'x' NOT LIKE 'y';

-- TEST: use LIKE in a SQL statement
-- +  _rc_ = cql_prepare(_db_, &_temp_stmt,
-- +  "SELECT 'x' LIKE 'y'"
SET i2 := (SELECT 'x' LIKE 'y');

-- TEST: use not LIKE in a SQL statement
-- +  _rc_ = cql_prepare(_db_, &_temp_stmt,
-- +  "SELECT 'x' NOT LIKE 'y'"
SET i2 := (SELECT 'x' NOT LIKE 'y');

-- TEST: use match in a SQL statement
-- +  _rc_ = cql_prepare(_db_, &_temp_stmt,
-- +  "SELECT 'x' MATCH 'y'"
SET i2 := (SELECT 'x' MATCH 'y');

-- TEST: use glob in a SQL statement
-- +  _rc_ = cql_prepare(_db_, &_temp_stmt,
-- +  "SELECT 'x' GLOB 'y'"
SET i2 := (SELECT 'x' GLOB 'y');

-- TEST: use lot of bitwise operators
-- NOTE the SQL order of ops is different...
-- no parens used here
-- +  SET i2 := 1 << 2 | 1 << 4 & 1 >> 8;
-- in Sqlite binary math operators all bind equal AND left to right so the above is the same as
--    SET i2 :=  (((((1 << 2) | 1) << 4) & 1) >> 8);
-- in C that becomes
-- because i C  << AND >> are stronger than | AND &
-- + i2 = ((1 << 2 | 1) << 4 & 1) >> 8;
SET i2 := 1 << 2 | 1 << 4 & 1 >> 8;

-- TEST: now maybe what you expected to see.  Force the issue WITH parens
-- + SET i2 := 1 << 2 | (1 << 4) & (1 >> 8);
-- Still not what you expected... remember | AND & are EQUAL in sql
-- so the above was parsed left to right...
-- + i2 = (1 << 2 | 1 << 4) & 1 >> 8;
SET i2 := (1 << 2) | (1 << 4) & (1 >> 8);

-- TEST: this is really the normal thing
-- some parens were redunant, removed...
-- + SET i2 := 1 << 2 | (1 << 4 & (1 >> 8));
-- now this is the usual C order of ops AND no parens are in the C
-- + i2 = 1 << 2 | 1 << 4 & 1 >> 8;
SET i2 := (1 << 2) | ((1 << 4) & (1 >> 8));

-- TEST: force a high binding ~
-- nothing weird here, ~ binds very strong in both languages
-- + i2 = 1 | ~ i2;
SET i2 := 1 | ~i2;

-- TEST: CREATE a trigger, force the dml
-- + _rc_ = cql_exec(_db_,
-- +   "CREATE TEMP TRIGGER IF NOT EXISTS trigger1 "
-- +     "BEFORE DELETE ON bar "
-- +     "FOR EACH ROW "
-- +     "WHEN old.id > 7 "
-- +   "BEGIN "
-- +     "SELECT old.id; "
-- +   "END"
CREATE PROC make_trigger()
BEGIN
  CREATE temp trigger IF not exists trigger1
    before DELETE on bar
    FOR each row
    when old.id > 7
  BEGIN
    SELECT old.id;
  END;
END;

-- TEST: IS patterns
-- + b = 1 == 1;
-- + b = cql_string_equal(_literal_%_x_, _literal_%_x_);
-- + b = cql_string_equal(_literal_%_x_, _literal_%_y_);
-- + b = !!(1 + (3 == 4));
-- + cql_set_notNULL(i, 1);
-- + cql_set_notNULL(j, 2);
-- + b = ((i.is_NULL == j.is_NULL) && (j.is_NULL || i.value == j.value))
CREATE PROC is_test()
BEGIN
  DECLARE b bool NOT NULL;
  SET b := 1 is 1;
  SET b := 'x' is 'x';
  SET b := 'x' is 'y';
  SET b := 1 + (3 is 4);

  LET i := NULLable(1);
  LET j := NULLable(2);

  SET b := i is j;
END;

-- TEST: blob comparaison
-- + b = cql_blob_equal(bl1, bl2);
-- + b = !cql_blob_equal(bl1, bl2);
CREATE PROC is_blob()
BEGIN
  DECLARE bl1 blob;
  DECLARE bl2 blob;
  DECLARE b bool NOT NULL;
  SET b := bl1 is bl2;
  SET b := bl1 is not bl2;
END;

-- TEST: IS NOT patterns
-- + b = 1 != 1;
-- + b = !cql_string_equal(_literal_%_x_, _literal_%_x_);
-- + b = !cql_string_equal(_literal_%_x_, _literal_%_y_);
-- + b = !!(1 + (3 != 4));
-- + cql_set_notNULL(i, 1);
-- + cql_set_notNULL(j, 2);
-- + b = !((i.is_NULL == j.is_NULL) && (j.is_NULL || i.value == j.value))
CREATE PROC is_not_test()
BEGIN
  DECLARE b bool NOT NULL;
  SET b := 1 is not 1;
  SET b := 'x' is not 'x';
  SET b := 'x' is not 'y';
  SET b := 1 + (3 is not 4);

  LET i := NULLable(1);
  LET j := NULLable(2);

  SET b := i is not j;
END;

-- TEST: NULL on lhs of IN
-- + cql_set_NULL(*b);
CREATE PROC in_test(x INTEGER, out b bool)
BEGIN
  SET b := NULL IN (1);
END;

-- TEST: NULL on lhs of NOT IN
-- + cql_set_NULL(*b);
-- + DECLARE PROC not_in_test (x INTEGER, OUT b BOOL);
CREATE PROC not_in_test(x INTEGER, out b bool)
BEGIN
  SET b := NULL NOT IN (1);
END;

-- TEST: drop a trigger (both flavors)
-- +1 "DROP TRIGGER IF EXISTS trigger1"
-- +1 "DROP TRIGGER trigger1"
CREATE PROC drop_trigger_test()
BEGIN
  drop trigger IF exists trigger1;
  drop trigger trigger1;
END;

-- TEST: CREATE PROC WITH a single-column identity attribute
-- + cql_uint16 simple_identity_identity_columns[] = { 1,
-- + DECLARE PROC simple_identity () (id INTEGER NOT NULL, data INTEGER NOT NULL);
@attribute(cql:identity=(id))
CREATE PROC simple_identity()
BEGIN
  SELECT 1 AS id, 2 AS data;
END;

-- TEST: CREATE PROC WITH a multi-column identity attribute
-- + cql_uint16 complex_identity_identity_columns[] = { 2,
@attribute(cql:identity=(col1, col2))
CREATE PROC complex_identity()
BEGIN
  SELECT 1 AS col1, 2 AS col2, 3 AS data;
END;

-- TEST: CREATE PROC WITH a out CURSOR AND identity column
-- + cql_uint16 out_cursor_identity_identity_columns[] = { 1,
@attribute(cql:identity=(id))
CREATE PROC out_cursor_identity()
BEGIN
  DECLARE C CURSOR FOR SELECT 1 AS id, 2 AS data;
  fetch C;
  out C;
END;

CREATE TABLE radioactive(
 id INTEGER NOT NULL,
 data text @sensitive
);

-- TEST: CREATE a PROC that reducts some sensitive data
-- + CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // id
-- + CQL_DATA_TYPE_STRING | CQL_DATA_TYPE_ENCODED, // data
-- + void radioactive_proc_set_encoding(cql_int32 col, cql_bool encode) {
@attribute(cql:vault_sensitive)
CREATE PROC radioactive_proc()
BEGIN
 SELECT * FROM radioactive;
END;

-- TEST: WITH DELETE form
-- + _rc_ = cql_exec(_db_,
-- +   "WITH "
-- +   "x (a) AS (SELECT 111) "
-- +   "DELETE FROM foo WHERE id IN (SELECT a "
-- +     "FROM x)");
CREATE PROC with_deleter()
BEGIN
  WITH x(a) AS (SELECT 111)
    DELETE FROM foo WHERE id in (SELECT * FROM x);
END;

-- TEST: WITH UPDATE form
-- + _rc_ = cql_exec(_db_,
-- +  "WITH "
-- +  "x (a) AS (SELECT 111) "
-- +  "UPDATE bar "
-- +  "SET name = 'xyzzy' "
-- +    "WHERE id IN (SELECT a "
-- +    "FROM x)");
CREATE PROC with_updater()
BEGIN
  WITH x(a) AS (SELECT 111)
    UPDATE bar SET name = 'xyzzy' WHERE id in (SELECT * FROM x);
END;

CREATE temp TABLE table1( id INTEGER);
CREATE temp TABLE table2( id INTEGER);

-- TEST: autodrop attribute
-- + .autodrop_tables = "table1\0table2\0",
@attribute(cql:autodrop=(table1, table2))
CREATE PROC autodropper()
BEGIN
   SELECT 1 a, 2 b;
END;

-- TEST: base fragment attribute
-- there should be no PROC codegen
-- - cql_code % base_fragment
-- - cleanup
@attribute(cql:base_fragment=core)
CREATE PROC base_fragment(id_ INTEGER NOT NULL)
BEGIN
with
  core(x,y,z) AS (SELECT id,name,rate FROM bar WHERE id = id_)
SELECT * FROM core;
END;

-- TEST: make sure that the name of the CURSOR is canonicalized
-- There should be no references to the version WITH the wrong case
-- + simple_cursor_proc_A_CURSOR_row A_CURSOR = { 0 };
-- + A_CURSOR._has_row_ = 1;
-- + A_CURSOR.id = 1;
-- + _result_->_has_row_ = A_CURSOR._has_row_;
-- + _result_->id = A_CURSOR.id;
CREATE procedure simple_cursor_proc()
BEGIN
  DECLARE A_CURSOR CURSOR LIKE SELECT 1 id;
  fetch a_cursor (id) FROM VALUES(1);
  out a_cursor;
END;

-- TEST: force codegen to include (and ignore) the enforcement directives
-- these have no output so there's nothing to verify really
-- we just verify that we did not echo the comment FOR these
-- thereby causing the need FOR the global PROC FOR no reason
-- - @enforce
@enforce_strict FOREIGN KEY ON UPDATE;
@enforce_normal FOREIGN KEY ON DELETE;

-- TEST: force codegen to include (and ignore) the schema region directives
-- these have no output so there's nothing to verify really
-- we just verify that we did not echo the comment FOR these
-- thereby causing the need FOR the global PROC FOR no reason
-- - @declare
-- - @BEGIN
-- - @end
-- - schema
-- - region
@declare_schema_region root_region;
@BEGIN_schema_region root_region;
@end_schema_region;

-- this section has trivial casts, we still need to test codegen FOR this
-- because normal mode is still legal
@enforce_normal cast;

-- TEST: SELECT WITH redundant cast AND alias
-- + "SELECT (5), T.xyzzy "
-- + "FROM (SELECT 1 AS xyzzy) AS T");
CREATE PROC redundant_CAST()
BEGIN
  SELECT CAST(5 AS INTEGER) plugh, T.xyzzy five FROM (SELECT 1 xyzzy) AS T;
END;

-- TEST: SELECT WITH alias in view
-- + "CREATE VIEW alias_preserved AS "
-- + "SELECT (5) AS plugh, T.xyzzy AS five "
-- + "FROM (SELECT 1 AS xyzzy) AS T");
CREATE PROC view_creator()
BEGIN
  CREATE VIEW alias_preserved as
    SELECT CAST(5 AS INTEGER) plugh, T.xyzzy five FROM (SELECT 1 xyzzy) AS T;
END;

@enforce_strict cast;

CREATE TABLE switch_account_badges(badge_count INTEGER);
CREATE TABLE unread_pending_threads(unread_pending_thread_count INTEGER);

-- TEST: nested SELECT TABLE should not have column aliases removed
-- +  "SELECT SUM(A.unread_pending_thread_count), SUM(A.switch_account_badge_count) "
-- +    "FROM (SELECT P.unread_pending_thread_count AS unread_pending_thread_count, 0 AS switch_account_badge_count "
-- +    "FROM unread_pending_threads AS P "
-- +  "UNION ALL "
-- +  "SELECT 0 AS unread_pending_thread_count, S.badge_count AS switch_account_badge_count "
-- +    "FROM switch_account_badges AS S) AS A");
CREATE PROC settings_info ()
BEGIN
  DECLARE C CURSOR for
    SELECT SUM(A.unread_pending_thread_count) AS unread_pending_thread_count,
         SUM(A.switch_account_badge_count) AS switch_account_badge_count
    FROM (SELECT P.unread_pending_thread_count AS unread_pending_thread_count, 0 AS switch_account_badge_count
    FROM unread_pending_threads AS P
  UNION ALL
  SELECT 0 AS unread_pending_thread_count, S.badge_count AS switch_account_badge_count
    FROM switch_account_badges AS S) AS A;
END;

-- TEST: aliases in top-level selects can be removed IF not referenced
-- + "SELECT 1, 2 "
-- + "UNION ALL "
-- + "SELECT foo.id, 2 "
-- +   "FROM foo");
CREATE PROC top_level_select_alias_unused()
BEGIN
  SELECT 1 AS id, 2 AS x
  UNION ALL
  SELECT foo.id, 2 AS x
  FROM foo;
END;

-- TEST: aliases in top-level selects must not be removed IF referenced FROM an
-- ORDER BY clause
-- + "SELECT 1 AS id, 2 "
-- + "UNION ALL "
-- + "SELECT foo.id, 2 "
-- +   "FROM foo "
-- + "ORDER BY id");
CREATE PROC top_level_select_alias_used_in_orderby()
BEGIN
  SELECT 1 AS id, 2 AS x
  UNION ALL
  SELECT foo.id, 2 AS x
  FROM foo
  ORDER BY id;
END;

-- TEST: try to use a WITH_SELECT form in a SELECT expression
-- +  _rc_ = cql_prepare(_db_, &_temp_stmt,
-- +   "WITH "
-- +   "threads2 (count) AS (SELECT 1) "
-- +   "SELECT COUNT(*) "
-- +     "FROM threads2");
CREATE PROC use_with_select()
BEGIN
   DECLARE x INTEGER;
   SET x := (WITH threads2 (count) AS (SELECT 1 foo) SELECT COUNT(*) FROM threads2);
END;

-- DECLARE a simple table-valued function
DECLARE SELECT FUNCTION ReadFromRowset(rowSET Object<rowset>) (id INTEGER);

-- TEST: use a TABLE valued FUNCTION that consumes an object
-- + cql_multibind(&_rc_, _db_, &C_stmt, 1,
-- + CQL_DATA_TYPE_OBJECT, rowset);
CREATE PROC rowset_object_reader(rowSET Object<rowset>)
BEGIN
  DECLARE C CURSOR FOR SELECT * FROM ReadFromRowset(rowset);
END;

-- TEST: codegen upsert statement WITH UPDATE statement
-- + "INSERT INTO foo(id) SELECT id "
-- + "FROM bar "
-- + "WHERE 1 "
-- + "ON CONFLICT (id) DO UPDATE "
-- + "SET id = 10 "
-- + "WHERE id <> 10"
-- + cql_code upsert_do_something(sqlite3 *_NonNULL _db_) {
CREATE PROC upsert_do_something()
BEGIN
 INSERT INTO foo SELECT id FROM bar WHERE 1 on conflict(id) do UPDATE SET id=10 WHERE id != 10;
END;

-- TEST: codegen WITH upsert statement form
-- + cql_code with_upsert_form(sqlite3 *_NonNULL _db_) {
-- + "WITH "
-- + "names (id) AS (VALUES(1), (5), (3), (12)) "
-- + "INSERT INTO foo(id) SELECT id "
-- +   "FROM names "
-- +   "WHERE 1 "
-- + "ON CONFLICT (id) DO UPDATE "
-- + "SET id = 10 "
-- +   "WHERE id <> 10");
CREATE PROC with_upsert_form()
BEGIN
 WITH names(id) AS (values (1), (5), (3), (12))
 INSERT INTO foo SELECT id FROM names WHERE 1 on conflict(id) do UPDATE SET id = 10 WHERE id != 10;
END;

-- TEST: codegen upsert statement WITH do nothing
-- + "INSERT INTO foo(id) VALUES(?) "
-- + "ON CONFLICT DO NOTHING"
-- + cql_code upsert_do_nothing(sqlite3 *_NonNULL _db_, cql_int32 id_) {
CREATE PROC upsert_do_nothing(id_ INTEGER NOT NULL)
BEGIN
 INSERT INTO foo(id) VALUES(id_) on conflict do nothing;
END;

-- TEST: codegen with-insert WITH a seed
-- + _seed_ = 1337;
-- + _rc_ = cql_exec(_db_,
-- + "WITH "
-- + "some_cte (id) AS (SELECT 1 AS id) "
-- + "INSERT INTO bar(id) VALUES(ifNULL(( SELECT id "
-- + "FROM some_cte ), 0))");
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
WITH some_cte(id) AS (SELECT 1 id)
INSERT INTO bar(id)
values (ifNULL((SELECT id FROM some_cte), 0))
@dummy_seed(1337);

-- TEST: codegen upsert WITH a seed
-- + _seed_ = 1338;
-- + _rc_ = cql_exec(_db_,
-- + "INSERT INTO bar(id) VALUES(1) "
-- + "ON CONFLICT (id) DO UPDATE "
-- + "SET id = 10");
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
INSERT INTO bar(id) VALUES(1) @dummy_seed(1338)
on conflict(id) do
UPDATE SET id=10;

-- TEST: SET up a couple of out CURSOR procs (body not needed)
DECLARE procedure p1() out (id INTEGER NOT NULL, t text);
DECLARE procedure p2() out (id INTEGER NOT NULL, t text) using transaction;

-- TEST: this test forces several out cursors to go into the symbol table
-- the idea is that it reveals any cases WHERE a temporary pointer is
-- stored into the symbol TABLE AS was the case WITH the temporary
-- row data FOR each cursor.  The test is this:  is c2 properly emitted?
-- + use_many_out_cursors_c1_row c1 = { ._refs_count_ = 1, ._refs_offset_ = use_many_out_cursors_c1_refs_offSET };
-- + use_many_out_cursors_c2_row c2 = { ._refs_count_ = 1, ._refs_offset_ = use_many_out_cursors_c2_refs_offSET };
-- +1 p1((p1_row *)&c1);
-- +1 _rc_ = p2(_db_, (p2_row *)&c2);
-- +2 cql_teardown_row(c1);
-- +2 cql_teardown_row(c2);
CREATE procedure use_many_out_cursors()
BEGIN
  DECLARE c1 CURSOR fetch FROM call p1();
  DECLARE c2 CURSOR fetch FROM call p2();
END;

-- TEST: each fetch forces the declaration of the CURSOR storage IF it has
-- not already been declared.  In this case the third branch of the if
-- must find that p1 AND p2 row data are already DECLARE AND not duplicate
-- the declarations.
-- +1 fetch_many_times_C_row C = { ._refs_count_ = 1, ._refs_offset_ = fetch_many_times_C_refs_offSET };
-- +2 p1((p1_row *)&C);
-- +2 _rc_ = p2(_db_, (p2_row *)&C);
-- +5 cql_teardown_row(C);
CREATE procedure fetch_many_times(arg bool NOT NULL)
BEGIN
  DECLARE C CURSOR LIKE p1;
  IF arg  == 1 then
    fetch C FROM call p1();
  else IF arg == 2 then
    fetch C FROM call p2();
  else
    fetch C FROM call p1();
    fetch C FROM call p2();
  end if;
END;

-- TEST: CREATE a result SET FROM rows values
-- + DECLARE PROC out_union_two () OUT UNION (x INTEGER NOT NULL, y TEXT NOT NULL);
-- + void out_union_two_fetch_results(out_union_two_result_set_ref _NULLable *_NonNULL _result_set_) {
-- + cql_profile_start(CRC_out_union_two, &out_union_two_perf_index);
-- + cql_bytebuf _rows_;
-- + cql_bytebuf_open(&_rows_);
-- +2 cql_retain_row(C);
-- +2 IF (C._has_row_) cql_bytebuf_append(&_rows_, (const void *)&C, sizeof(C));
-- + cql_results_from_data(SQLITE_OK, &_rows_, &out_union_two_info, (cql_result_set_ref *)_result_set_);
-- + cql_teardown_row(C);
CREATE PROC out_union_two()
BEGIN
 DECLARE C CURSOR LIKE SELECT 1 x, '2' y;
 fetch C FROM VALUES(1, "y");
 out union C;
 out union C;
END;

-- TEST: read back the two rows FROM the above
-- + CQL_WARN_UNUSED cql_code out_union_reader(sqlite3 *_NonNULL _db_) {
-- + out_union_two_result_set_ref c_result_set_ = NULL
-- + cql_int32 c_row_num_ = 0;
-- + cql_int32 c_row_count_ = 0;
-- + out_union_reader_c_row c = { ._refs_count_ = 1, ._refs_offset_ = out_union_reader_c_refs_offSET };
-- + out_union_two_fetch_results(&c_result_set_);
-- + c_row_num_ = c_row_count_ = -1;
-- + c_row_count_ = cql_result_set_get_count((cql_result_set_ref)c_result_set_);
-- + FOR (;;) {
-- +   c_row_num_++;
-- +   c._has_row_ = c_row_num_ < c_row_count_;
-- +   cql_copyoutrow(NULL, (cql_result_set_ref)C_result_set_, c_row_num_, 2,
-- +                  CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &c.x,
-- +                  CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_STRING, &c.y);
-- NOT PRESENT !!
-- -   IF (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
-- +   IF (!c._has_row_) break;
-- + }
-- + cql_object_release(c_result_set_);
-- + cql_teardown_row(c);
-- + return _rc_;
CREATE PROC out_union_reader()
BEGIN
  DECLARE c CURSOR FOR call out_union_two();
  loop fetch C
  BEGIN
    call printf("%d %s\n", C.x, C.y);
  END;
END;

-- TEST: CREATE a result SET FROM selected rows
-- + DECLARE PROC out_union_from_SELECT () OUT UNION (x INTEGER NOT NULL, y TEXT NOT NULL) USING TRANSACTION;
-- + cql_bytebuf _rows_;
-- + cql_bytebuf_open(&_rows_);
-- +2 cql_retain_row(C);
-- +2 IF (C._has_row_) cql_bytebuf_append(&_rows_, (const void *)&C, sizeof(C));
-- + out_union_from_select_info.db = _db_;
-- + cql_results_from_data(_rc_, &_rows_, &out_union_from_select_info, (cql_result_set_ref *)_result_set_);
-- + cql_teardown_row(C);
CREATE PROC out_union_from_select()
BEGIN
 DECLARE C CURSOR FOR SELECT 1 x, '2' y;
 fetch C;
 out union C;
 out union C;
END;

-- TEST: reading FROM out union again, this time a DML PROC (uses select)
-- slightly different call path
-- + _rc_ = out_union_from_select_fetch_results(_db_, &c_result_set_);
-- + c_row_num_ = c_row_count_ = -1;
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
-- + c_row_count_ = cql_result_set_get_count((cql_result_set_ref)c_result_set_);
CREATE PROC out_union_dml_reader()
BEGIN
  DECLARE c CURSOR FOR call out_union_from_select();
  loop fetch C
  BEGIN
    call printf("%d %s\n", C.x, C.y);
  END;
END;


-- This just sets up a call to a procedure that takes two INTEGERs
CREATE PROC out_union_VALUES(a INTEGER NOT NULL, b INTEGER NOT NULL)
BEGIN
  DECLARE x CURSOR LIKE SELECT 1 x, 2 y;
  fetch x FROM VALUES(a,b);
  out union x;
END;

-- TEST:  we need to be able to call the above proc, this requires
-- the args be emitted correctly, WITH a comma (!)
-- + out_union_values_fetch_results(&C_result_set_, a, b);
-- + C_row_num_ = C_row_count_ = -1;
-- + C_row_count_ = cql_result_set_get_count((cql_result_set_ref)C_result_set_);
-- + C_row_num_++;
-- + C._has_row_ = C_row_num_ < C_row_count_;
-- + cql_copyoutrow(NULL, (cql_result_set_ref)C_result_set_, C_row_num_, 2,
-- +   CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &C.x,
-- +   CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &C.y);
CREATE PROC read_out_union_VALUES(a INTEGER NOT NULL, b INTEGER NOT NULL)
BEGIN
  DECLARE C CURSOR FOR call out_union_VALUES(a,b);
  fetch C;
END;

-- This just sets up a call to a procedure that proceduce a dml out union result set
-- + out_union_dml_info.db = _db_;
-- + cql_results_from_data(_rc_, &_rows_, &out_union_dml_info, (cql_result_set_ref *)_result_set_);
@attribute(cql:vault_sensitive)
CREATE PROC out_union_dml()
BEGIN
  DECLARE x CURSOR FOR SELECT * FROM radioactive;
  fetch x;
  out union x;
END;

-- TEST: we need to make sure the a notNULL db pointer is pass to cql_copyoutrow(...)
-- + cql_copyoutrow(_db_, (cql_result_set_ref)C_result_set_, C_row_num_, 2,
-- +                CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_INT32, &C.id,
-- +                CQL_DATA_TYPE_STRING, &C.data);
@attribute(cql:vault_sensitive)
CREATE PROC out_union_dml_for_call()
BEGIN
  DECLARE C CURSOR FOR call out_union_dml();
  fetch C;
END;

-- TEST: generate a compound SELECT statement in an expression (this is a legal form)
-- + _rc_ = cql_prepare(_db_, &_temp_stmt,
-- + "SELECT 1 "
-- + "WHERE 0 "
-- + "UNION "
-- + "SELECT 2 "
-- + "LIMIT 1");
CREATE PROC compound_select_expr()
BEGIN
  DECLARE x INTEGER;
  SET x := (SELECT 1 WHERE 0 union SELECT 2 LIMIT 1);
END;

-- TEST: generate window FUNCTION invocation
-- + "SELECT id,  "
-- + "row_number() OVER () "
-- + "FROM foo");
CREATE PROC window_function_invocation()
BEGIN
  SELECT id, row_number() over () AS row_num FROM foo;
END;

-- TEST: UPDATE some of the CURSOR columns
-- + IF (C._has_row_) {
-- +   C.x = 2;
-- + }
CREATE PROC update_cursor()
BEGIN
  DECLARE C CURSOR LIKE SELECT 1 x, 2 y;
  UPDATE CURSOR C(x) FROM values (2);
END;

-- TEST: make sure decl output is correct FOR DML out union
-- + DECLARE PROC out_union_with_dml (id INTEGER) OUT UNION (id INTEGER NOT NULL) USING TRANSACTION;
DECLARE PROC out_union_with_dml(id INTEGER) out union (id INTEGER NOT NULL) using transaction;

-- TEST: make sure decl output is correct FOR non-DML out union
-- + DECLARE PROC out_union_no_dml (id INTEGER) OUT UNION (id INTEGER NOT NULL);
DECLARE PROC out_union_no_dml(id INTEGER) out union (id INTEGER NOT NULL);

-- TEST: emit goto cql_cleanup in case of return
-- + goto cql_cleanup; // return
-- + cql_cleanup:
CREATE PROC use_return()
BEGIN
  BEGIN try
    SELECT 1 x;
  end try;
  BEGIN catch
    return;
  end catch;
END;

-- TEST: emit goto cql_cleanup in case of return, force the label even IF not
-- used FOR any other error processing
-- + goto cql_cleanup; // return
-- + cql_cleanup:
CREATE PROC use_return_no_error_flow()
BEGIN
  IF 1 then
    return;
  end if;
END;

-- TEST: empty PROC body
-- + CREATE PROC empty_proc ()
-- + BEGIN
-- + END;
-- + void empty_proc(void) {
-- - cql_cleanup:
-- + }
CREATE PROC empty_proc()
BEGIN
END;

-- TEST: empty body parts, all statement list types
-- nothing really to validate here; IF any of the empty cases
-- are not handled it will crash.  IF the blocks are badly shaped
-- it won't compile. Can't think of anything that isn't redundant here
-- + CREATE PROC empty_blocks ()
CREATE PROC empty_blocks()
BEGIN
  IF 1 then
  end if;

  IF 2 then
  else
  end if;

  IF 3 then
  else IF 4
  then
  else
  end if;

  WHILE 1
  BEGIN
  END;

  DECLARE c CURSOR FOR SELECT 1 x;
  loop fetch c
  BEGIN
  END;

  BEGIN try
  end try;
  BEGIN catch
  end catch;
END;

-- This PROC illustrates a case WHERE we need to put the ;
-- after the catch label so that there is a statement.  Note
-- the problem is that two catches end in a row.  The fact that
-- the bodies of the functions are empty changes nothing. It only
-- matters that we had end catch AND then end some other block
-- so empty statement lists are not required to make this issue happen
-- + CQL_WARN_UNUSED cql_code tail_catch(sqlite3 *_NonNULL _db_) {
-- +2 // try
-- + goto catch_end_6;
-- + goto catch_end_7;
-- mandatory ; after this label
-- + catch_end_7:;
-- + catch_end_6:;
CREATE PROC tail_catch()
BEGIN
   BEGIN try
   end try;
   BEGIN catch
     BEGIN try
     end try;
     BEGIN catch
     end catch;
   end catch;
END;

-- TEST: the SQL output will include an escaped quote ''
-- this used to fool us into thinking we had left quoted mode
-- AND so later characters would not be correctly escaped in the output
-- in particular the newline would get messed up because we thought
-- it was a line break in non-quoted SQL which can be replaced WITH a space
-- note the newline is escaped AND present
-- + "INSERT INTO bar(id, name) VALUES(1, 'it''s high noon\r\n\f\b\t\v')");
CREATE PROC pretty_print_with_quote()
BEGIN
  INSERT INTO bar(id, name) VALUES(1, "it's high noon\r\n\f\b\t\v");
END;

-- TEST: string literal WITH hex forms
-- + "INSERT INTO bar(id, name) VALUES(1, '\x01\x02\xa1\x1bg')");
CREATE PROC hex_quote()
BEGIN
  INSERT INTO bar(id, name) VALUES(1, "\x01\x02\xA1\x1b\x00\xg");
END;

-- TEST: no getters generated FOR this function
-- getters go into the .h stream AND there's no test hook FOR that
-- but this can be verified by checking the .ref FOR the header file manually
-- We verify that we are still generating the data types (only the getters are suppressed)
-- +  CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // id
-- +  CQL_DATA_TYPE_STRING, // name
-- +  CQL_DATA_TYPE_INT64, // rate
-- +  CQL_DATA_TYPE_INT32, // type
-- +  CQL_DATA_TYPE_DOUBLE, // size
@attribute(cql:suppress_getters)
CREATE PROC lotsa_columns_no_getters()
BEGIN
  SELECT * FROM bar;
END;


-- TEST: a copy FUNCTION will be generated
-- + cql_code sproc_with_copy(sqlite3 *_NonNULL _db_, sqlite3_stmt *_NULLable *_NonNULL _result_stmt)
@attribute(cql:generate_copy)
CREATE PROC sproc_with_copy()
BEGIN
  SELECT * FROM bar;
END;

-- TEST: emit an object result SET WITH setters WITH NOT NULL values
-- + extern void emit_object_with_setters_set_o(emit_object_with_setters_result_set_ref _NonNULL result_set, cql_object_ref _NonNULL new_value) {
-- +   cql_contract_argument_notNULL((void *)new_value, 2);
-- +   cql_result_set_set_object_col((cql_result_set_ref)result_set, 0, 0, new_value);
-- + extern void emit_object_with_setters_set_x(emit_object_with_setters_result_set_ref _NonNULL result_set, cql_object_ref _NonNULL new_value) {
-- +   cql_contract_argument_notNULL((void *)new_value, 2);
-- +   cql_result_set_set_object_col((cql_result_set_ref)result_set, 0, 1, new_value);
-- + extern void emit_object_with_setters_set_i(emit_object_with_setters_result_set_ref _NonNULL result_set, cql_int32 new_value) {
-- +   cql_result_set_set_int32_col((cql_result_set_ref)result_set, 0, 2, new_value);
-- + extern void emit_object_with_setters_set_l(emit_object_with_setters_result_set_ref _NonNULL result_set, cql_int64 new_value) {
-- +   cql_result_set_set_int64_col((cql_result_set_ref)result_set, 0, 3, new_value);
-- + extern void emit_object_with_setters_set_b(emit_object_with_setters_result_set_ref _NonNULL result_set, cql_bool new_value) {
-- +   cql_result_set_set_bool_col((cql_result_set_ref)result_set, 0, 4, new_value);
-- + extern void emit_object_with_setters_set_d(emit_object_with_setters_result_set_ref _NonNULL result_set, cql_double new_value) {
-- +   cql_result_set_set_double_col((cql_result_set_ref)result_set, 0, 5, new_value);
-- + extern void emit_object_with_setters_set_t(emit_object_with_setters_result_set_ref _NonNULL result_set, cql_string_ref _NonNULL new_value) {
-- +   cql_contract_argument_notNULL((void *)new_value, 2);
-- +   cql_result_set_set_string_col((cql_result_set_ref)result_set, 0, 6, new_value);
-- + extern void emit_object_with_setters_set_bl(emit_object_with_setters_result_set_ref _NonNULL result_set, cql_blob_ref _NonNULL new_value) {
-- +   cql_contract_argument_notNULL((void *)new_value, 2);
-- +   cql_result_set_set_blob_col((cql_result_set_ref)result_set, 0, 7, new_value);
@attribute(cql:emit_setters)
CREATE PROC emit_object_with_setters(
  o object NOT NULL,
  x object NOT NULL,
  i INTEGER NOT NULL,
  l long NOT NULL,
  b bool NOT NULL,
  d real NOT NULL,
  t text NOT NULL,
  bl blob NOT NULL)
BEGIN
  DECLARE C CURSOR LIKE emit_object_with_setters arguments;
  fetch C FROM arguments;
  out C;
END;

-- TEST: emit an object result SET WITH setters WITH NULLable values
-- + extern void emit_setters_with_NULLables_set_o(emit_setters_with_NULLables_result_set_ref _NonNULL result_set, cql_object_ref _NULLable new_value) {
-- +   cql_result_set_set_object_col((cql_result_set_ref)result_set, 0, 0, new_value);
-- + extern void emit_setters_with_NULLables_set_x(emit_setters_with_NULLables_result_set_ref _NonNULL result_set, cql_object_ref _NULLable new_value) {
-- +   cql_result_set_set_object_col((cql_result_set_ref)result_set, 0, 1, new_value);
-- + extern void emit_setters_with_NULLables_set_i_value(emit_setters_with_NULLables_result_set_ref _NonNULL result_set, cql_int32 new_value) {
-- +   cql_result_set_set_int32_col((cql_result_set_ref)result_set, 0, 2, new_value);
-- + extern void emit_setters_with_NULLables_set_i_to_NULL(emit_setters_with_NULLables_result_set_ref _NonNULL result_set) {
-- +   cql_result_set_set_int32_col((cql_result_set_ref)result_set, 0, 2, new_value);
-- + extern void emit_setters_with_NULLables_set_l_value(emit_setters_with_NULLables_result_set_ref _NonNULL result_set, cql_int64 new_value) {
-- +   cql_result_set_set_int64_col((cql_result_set_ref)result_set, 0, 3, new_value);
-- + extern void emit_setters_with_NULLables_set_l_to_NULL(emit_setters_with_NULLables_result_set_ref _NonNULL result_set) {
-- +   cql_result_set_set_int64_col((cql_result_set_ref)result_set, 0, 3, new_value);
-- + extern void emit_setters_with_NULLables_set_b_value(emit_setters_with_NULLables_result_set_ref _NonNULL result_set, cql_bool new_value) {
-- +   cql_result_set_set_bool_col((cql_result_set_ref)result_set, 0, 4, new_value);
-- + extern void emit_setters_with_NULLables_set_b_to_NULL(emit_setters_with_NULLables_result_set_ref _NonNULL result_set) {
-- +   cql_result_set_set_bool_col((cql_result_set_ref)result_set, 0, 4, new_value);
-- + extern void emit_setters_with_NULLables_set_d_value(emit_setters_with_NULLables_result_set_ref _NonNULL result_set, cql_double new_value) {
-- +   cql_result_set_set_double_col((cql_result_set_ref)result_set, 0, 5, new_value);
-- + extern void emit_setters_with_NULLables_set_d_to_NULL(emit_setters_with_NULLables_result_set_ref _NonNULL result_set) {
-- +   cql_result_set_set_double_col((cql_result_set_ref)result_set, 0, 5, new_value);
-- + extern void emit_setters_with_NULLables_set_t(emit_setters_with_NULLables_result_set_ref _NonNULL result_set, cql_string_ref _NULLable new_value) {
-- +   cql_result_set_set_string_col((cql_result_set_ref)result_set, 0, 6, new_value);
-- + extern void emit_setters_with_NULLables_set_bl(emit_setters_with_NULLables_result_set_ref _NonNULL result_set, cql_blob_ref _NULLable new_value) {
-- +  cql_result_set_set_blob_col((cql_result_set_ref)result_set, 0, 7, new_value);
@attribute(cql:emit_setters)
CREATE PROC emit_setters_with_NULLables(
  o object,
  x object,
  i INTEGER,
  l long,
  b bool,
  d real,
  t text,
  bl blob)
BEGIN
  DECLARE C CURSOR LIKE emit_setters_with_NULLables arguments;
  fetch C FROM arguments;
  out C;
END;


-- TEST: emit an object result SET not out AND setters
-- + extern void no_out_with_setters_set_id(no_out_with_setters_result_set_ref _NonNULL result_set, cql_int32 row, cql_int32 new_value)
-- + extern void no_out_with_setters_set_name(no_out_with_setters_result_set_ref _NonNULL result_set, cql_int32 row, cql_string_ref _NULLable new_value)
-- + extern void no_out_with_setters_set_rate_value(no_out_with_setters_result_set_ref _NonNULL result_set, cql_int32 row, cql_int64 new_value)
-- + extern void no_out_with_setters_set_type_value(no_out_with_setters_result_set_ref _NonNULL result_set, cql_int32 row, cql_int32 new_value)
-- + extern void no_out_with_setters_set_size_value(no_out_with_setters_result_set_ref _NonNULL result_set, cql_int32 row, cql_double new_value)
@attribute(cql:emit_setters)
CREATE PROC no_out_with_setters()
BEGIN
  SELECT * FROM bar;
END;


-- TEST: no result SET items should be generated at all
-- - CQL_DATA_TYPE
-- - lotsa_columns_no_result_set_fetch_results
-- - lotsa_columns_no_result_set_get_
-- - lotsa_columns_no_result_set_data_types
-- - lotsa_columns_no_result_set_refs_offset
-- - lotsa_columns_no_result_set_col_offsets
-- - lotsa_columns_no_result_set_result_count
@attribute(cql:suppress_result_set)
CREATE PROC lotsa_columns_no_result_set()
BEGIN
  SELECT * FROM bar;
END;

-- TEST: make sure that _rc_ is SET to SQLITE_OK when we return
-- + _rc_ = SQLITE_OK; // clean up any SQLITE_ROW value OR other non-error
-- + goto cql_cleanup; // return
CREATE PROC early_out_rc_cleared(out x INTEGER)
BEGIN
  DECLARE C CURSOR FOR SELECT 1 x;
  fetch C;
  IF C then
    return;
  end if;
END;

-- TEST: helper table
CREATE TABLE vault_mixed_sensitive(
  id int NOT NULL primary key,
  name text @sensitive,
  title text,
  type long @sensitive
);

CREATE TABLE vault_mixed_not_NULLable_sensitive(
  id int NOT NULL primary key,
  name text NOT NULL @sensitive,
  title text NOT NULL,
  type long NOT NULL @sensitive
);

-- TEST: helper table
CREATE TABLE vault_non_sensitive(
  id int NOT NULL primary key,
  name text,
  title text,
  type long
);

-- TEST: vault_sensitive attribute includes sensitive column (name) AND non sensitive column (id)
-- + CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // id
-- + CQL_DATA_TYPE_STRING | CQL_DATA_TYPE_ENCODED, // name
-- + CQL_DATA_TYPE_STRING, // title
-- + CQL_DATA_TYPE_INT64, // type
@attribute(cql:vault_sensitive=(id, name))
@attribute(cql:custom_type_for_encoded_column)
CREATE PROC vault_sensitive_with_values_proc()
BEGIN
 SELECT * FROM vault_mixed_sensitive;
END;

-- TEST: vault_sensitive attribute includes sensitive column (name) AND non sensitive column (id)
-- + CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // id
-- + CQL_DATA_TYPE_STRING | CQL_DATA_TYPE_NOT_NULL | CQL_DATA_TYPE_ENCODED, // name
-- + CQL_DATA_TYPE_STRING | CQL_DATA_TYPE_NOT_NULL, // title
-- + CQL_DATA_TYPE_INT64 | CQL_DATA_TYPE_NOT_NULL, // type
@attribute(cql:vault_sensitive=(id, name))
@attribute(cql:custom_type_for_encoded_column)
CREATE PROC vault_not_NULLable_sensitive_with_values_proc()
BEGIN
 SELECT * FROM vault_mixed_not_NULLable_sensitive;
END;

-- TEST: vault_sensitive attribute includes sensitive column (data) AND non sensitive column (id)
-- + CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // id
-- + CQL_DATA_TYPE_STRING | CQL_DATA_TYPE_ENCODED, // name
-- + CQL_DATA_TYPE_STRING, // title
-- + CQL_DATA_TYPE_INT64 | CQL_DATA_TYPE_ENCODED, // type
@attribute(cql:vault_sensitive)
CREATE PROC vault_sensitive_with_no_values_proc()
BEGIN
 SELECT * FROM vault_mixed_sensitive;
END;

-- TEST: vault union all a sensitive AND non sensitive table
-- + CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // id
-- + CQL_DATA_TYPE_STRING | CQL_DATA_TYPE_ENCODED, // name
-- + CQL_DATA_TYPE_STRING, // title
-- + CQL_DATA_TYPE_INT64 | CQL_DATA_TYPE_ENCODED, // type
@attribute(cql:vault_sensitive)
CREATE PROC vault_union_all_table_proc()
BEGIN
 SELECT * FROM vault_mixed_sensitive
 union all
 SELECT * FROM vault_non_sensitive;
END;

-- TEST: vault on alias column name
-- + CQL_DATA_TYPE_STRING | CQL_DATA_TYPE_ENCODED, // alias_name
@attribute(cql:vault_sensitive=alias_name)
CREATE PROC vault_alias_column_proc()
BEGIN
 SELECT name AS alias_name FROM vault_mixed_sensitive;
END;

-- TEST: vault on alias column name
-- + CQL_DATA_TYPE_STRING | CQL_DATA_TYPE_ENCODED, // alias_name
@attribute(cql:vault_sensitive=alias_name)
CREATE PROC vault_alias_column_name_proc()
BEGIN
 SELECT name AS alias_name FROM vault_mixed_sensitive;
END;

-- TEST: vault a column in CURSOR result
-- + cql_multifetch(_rc_, C_stmt, 1,
-- +                CQL_DATA_TYPE_STRING, &C.name);
@attribute(cql:vault_sensitive)
CREATE PROC vault_cursor_proc()
BEGIN
  DECLARE C CURSOR FOR SELECT name FROM vault_mixed_sensitive;
  fetch c;
END;

-- TEST: vault_sensitive attribute includes encode context column (title) AND sensitive column (id, name)
-- + CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // id
-- + CQL_DATA_TYPE_STRING | CQL_DATA_TYPE_ENCODED, // name
-- + CQL_DATA_TYPE_STRING, // title
-- + CQL_DATA_TYPE_INT64, // type
@attribute(cql:vault_sensitive=(title, (id, name)))
CREATE PROC vault_sensitive_with_context_and_sensitive_columns_proc()
BEGIN
 SELECT * FROM vault_mixed_sensitive;
END;

-- TEST: vault_sensitive attribute includes no encode context column AND sensitive column (id, name)
-- + CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // id
-- + CQL_DATA_TYPE_STRING | CQL_DATA_TYPE_ENCODED, // name
-- + CQL_DATA_TYPE_STRING, // title
-- + CQL_DATA_TYPE_INT64, // type
@attribute(cql:vault_sensitive=((id, name)))
CREATE PROC vault_sensitive_with_no_context_and_sensitive_columns_proc()
BEGIN
 SELECT * FROM vault_mixed_sensitive;
END;

-- TEST: vault_sensitive attribute includes encode context column (title) AND no sensitive column
-- + CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // id
-- + CQL_DATA_TYPE_STRING, // name
-- + CQL_DATA_TYPE_STRING, // title
-- + CQL_DATA_TYPE_INT64, // type
@attribute(cql:vault_sensitive=(title, (id, name)))
CREATE PROC vault_sensitive_with_context_and_no_sensitive_columns_proc()
BEGIN
 SELECT * FROM vault_non_sensitive;
END;

CREATE TABLE ext_test_table (
  f1 INTEGER NOT NULL,
  f2 INTEGER NOT NULL @sensitive,
  f3 INTEGER NOT NULL
);

-- TEST: define a base fragment, no output FOR this
-- - get
-- - set
-- - void
-- - return
@attribute(cql:base_fragment=frag_test)
CREATE PROC baseline()
BEGIN
  with
    frag_test(*) AS (SELECT 1 id)
  SELECT * FROM frag_test;
END;

-- TEST: extension creates no getters
-- - ext_get_id
-- - get_f2_is_NULL
-- - get_f2_value
-- - result_count
@attribute(cql:extension_fragment=frag_test)
@attribute(cql:vault_sensitive)
CREATE PROC ext()
BEGIN
  with
    frag_test(*) AS (SELECT 1 id),
    ext(*) AS (SELECT frag_test.*, f2 FROM frag_test left outer join ext_test_table on f1 = id)
  SELECT * FROM ext;
END;

-- TEST: another extension, this one should not include anything about f2, it doesn't "know" about that column
-- - f2
@attribute(cql:extension_fragment=frag_test)
CREATE PROC ext2()
BEGIN
  with
    frag_test(*) AS (SELECT 1 id),
    ext2(*) AS (SELECT frag_test.*, f3 FROM frag_test left outer join ext_test_table on f1 = id)
  SELECT * FROM ext2;
END;

-- TEST: simple box operation
-- + sqlite3_stmt *C_stmt = NULL;
-- + cql_object_ref C_object_ = NULL;
-- 1 FOR the boxing AND 1 FOR the cleanup
-- +2 cql_object_release(C_object_);
-- + C_object_ = cql_box_stmt(C_stmt);
-- + cql_set_object_ref(result, C_object_);
-- - cql_finalize_stmt(&C);
CREATE PROC try_boxing(out result object<bar cursor>)
BEGIN
  DECLARE C CURSOR FOR SELECT * FROM bar;
  SET result FROM CURSOR C;
END;

-- TEST: simple unbox
-- + C_stmt = cql_unbox_stmt(C_object_);
-- + cql_set_object_ref(&C_object_, boxed_cursor);
-- + _rc_ = sqlite3_step(C_stmt);
-- + cql_object_release(C_object_);
-- - cql_finalize_stmt(&C);
CREATE PROC try_unboxing(boxed_cursor object<bar cursor>)
BEGIN
  DECLARE C CURSOR FOR boxed_cursor;
  fetch C;
END;

-- TEST: numeric cast operation int32
-- + x = ((cql_int32)(3.2));
CREATE PROC local_cast_int_notNULL()
BEGIN
  DECLARE x INTEGER NOT NULL;
  SET x := CAST(3.2 AS INTEGER);
END;

-- TEST: numeric cast operation int32 NULLable
-- + cql_set_NULLable(x, r.is_NULL, ((cql_int32)(r.value)));
CREATE PROC local_cast_int()
BEGIN
  DECLARE x INTEGER;
  DECLARE r real;
  SET r := NULLable(3.2);
  SET x := CAST(r AS INTEGER);
END;

-- TEST: numeric cast operation int64 NULLable
-- + x = ((cql_int64)(3.2));
CREATE PROC local_cast_long_notNULL()
BEGIN
  DECLARE x long NOT NULL;
  SET x := CAST(3.2 AS long);
END;

-- TEST: numeric cast operation int64 NULLable
-- + cql_set_NULLable(x, r.is_NULL, ((cql_int64)(r.value)));
CREATE PROC local_cast_long()
BEGIN
  DECLARE x long;
  DECLARE r real;
  SET r := NULLable(3.2);
  SET x := CAST(r AS long);
END;

-- TEST: numeric cast operation real
-- + x = ((cql_double)(3));
CREATE PROC local_cast_real_notNULL()
BEGIN
  DECLARE x real NOT NULL;
  SET x := CAST(3 AS real);
END;

-- TEST: numeric cast operation real NULLable
-- + cql_set_NULLable(x, r.is_NULL, ((cql_double)(r.value)));
CREATE PROC local_cast_real()
BEGIN
  DECLARE x real;
  DECLARE r int;
  SET r := NULLable(3);
  SET x := CAST(r AS real);
END;

-- TEST: numeric cast operation bool (and normalize)
-- + x = ((cql_bool)!!(3.2));
CREATE PROC local_cast_bool_notNULL()
BEGIN
  DECLARE x bool NOT NULL;
  SET x := CAST(3.2 AS bool);
END;

-- TEST: numeric cast operation bool NULLable (and normalize)
-- + cql_set_NULLable(x, r.is_NULL, ((cql_bool)!!(r.value)));
CREATE PROC local_cast_bool()
BEGIN
  DECLARE x bool;
  DECLARE r real;
  SET r := NULLable(3.2);
  SET x := CAST(r AS bool);
END;

-- TEST: numeric cast operation FROM bool (normalize b)
-- + x = ((cql_double)!!(b));
CREATE PROC local_cast_from_bool_notNULL()
BEGIN
  DECLARE b bool NOT NULL;
  SET b := 1;
  DECLARE x real NOT NULL;
  SET x := CAST(b AS real);
END;

-- TEST: numeric cast operation FROM bool NULLable (normalize b)
-- + cql_set_NULLable(x, b.is_NULL, ((cql_double)!!(b.value)));
CREATE PROC local_cast_from_bool()
BEGIN
  DECLARE b bool;
  SET b := NULLable(1);
  DECLARE x real;
  SET x := CAST(b AS real);
END;

-- this section has trivial casts, we still need to test codegen FOR this
-- because normal mode is still legal
@enforce_normal cast;

-- TEST: numeric cast operation FROM bool NOT NULLable (no-op version)
-- + x = b;
CREATE PROC local_cast_from_bool_no_op_notNULL()
BEGIN
  DECLARE x bool NOT NULL;
  DECLARE b bool NOT NULL;
  SET b := 1;
  SET x := CAST(b AS bool);
END;

-- TEST: numeric cast operation FROM bool NULLable (no-op version)
-- + cql_set_NULLable(x, b.is_NULL, b.value);
CREATE PROC local_cast_from_bool_no_op()
BEGIN
  DECLARE b bool;
  SET b := NULLable(1);
  DECLARE x bool;
  SET x := CAST(b AS bool);
END;

@enforce_strict cast;

-- TEST: test cql_get_blob_size codegen
-- + cql_set_notNULL(l0_NULLable, cql_get_blob_size(_tmp_n_blob_0));
SET l0_NULLable := cql_get_blob_size((SELECT blob_var));

-- TEST: test cql_get_blob_size codegen WITH NOT NULL blob
-- + l2 = cql_get_blob_size(blob_var2);
SET l2 := cql_get_blob_size(blob_var2);

-- TEST: test basic PROC savepoint structure
-- + "SAVEPOINT base_proc_savepoint");
-- + // try
-- + "RELEASE base_proc_savepoint");
-- + catch_start% {
-- + "ROLLBACK TO base_proc_savepoint");
-- + _rc_ = cql_best_error(_rc_thrown_1);
-- + catch_end%:;
CREATE PROC base_proc_savepoint()
BEGIN
  PROC savepoint
  BEGIN
    DECLARE X INTEGER;
  END;
END;

-- TEST: commit returns will have two commit  paths
-- +1 "SAVEPOINT base_proc_savepoint_commit_return"
-- +3 "RELEASE base_proc_savepoint_commit_return"
-- +1 "ROLLBACK TO base_proc_savepoint_commit_return"
CREATE PROC base_proc_savepoint_commit_return()
BEGIN
  PROC savepoint
  BEGIN
    IF 1 then
      commit return;
    end if;
  END;
END;

-- TEST: rollback returns will have two rollback paths
-- +1 "SAVEPOINT base_proc_savepoint_rollback_return"
-- +2 "ROLLBACK TO base_proc_savepoint_rollback_return"
-- +3 "RELEASE base_proc_savepoint_rollback_return"
CREATE PROC base_proc_savepoint_rollback_return()
BEGIN
  PROC savepoint
  BEGIN
    IF 1 then
      rollback return;
    end if;
  END;
END;

DECLARE x INTEGER NOT NULL;

-- TEST: a series of paren checks on left association
-- avoid hard coded divide by zero
-- + x = 1 * (4 / 3);
SET x := 1 * (4 / 3);

-- + x = 1 * 2 / 3;
SET x := 1 * 2 / 3;

-- + x = 1 + 2 / 3;
SET x := 1 + 2 / 3;

-- + x = 1 + (2 - 3);
SET x := 1 + (2 - 3);

-- + x = 1 + 2 * 3;
SET x := 1 + 2 * 3;

-- + x = 1 * (2 + 3);
SET x := 1 * (2 + 3);

-- + x = 1 - (2 + 3);
SET x := 1 - (2 + 3);

-- + x = 1 - (2 - 3);
SET x := 1 - (2 - 3);

-- + x = 1 - 2 - (2 - 3);
SET x := 1 - 2 - (2 - 3);

-- the first parens do not change eval order FROM left to right at all
-- + x = 1 - 2 - (2 - 3);
SET x := (1 - 2) - (2 - 3);

-- + x = 1 / 2 / 3;
SET x := 1 / 2 / 3;

-- avoid hard coded divide by zero
-- + x = 1 / (4 / 3);
SET x := 1 / (4 / 3);

-- + x = 1 / 2;
SET x := 1 / 2;

-- + x = 1 * 2 * (3 * 4)
SET x := 1 * 2 * (3 * 4);

-- the first parens don't change anything
-- the second parens could matter IF it was floating point
-- + x = 1 * 2 * (3 * 4)
SET x := (1 * 2) * (3 * 4);

-- note that in C & binds tighter than | so parens are required in C
-- note that in SQL | AND & are equal so this expression left associates
-- + x = (1 | 2) & 3;
SET x := 1 | 2 & 3;

-- + x = 1 | 2 & 3;
SET x := 1 | (2 & 3);

-- + x = 1 | 2 | 3
SET x := 1 | 2 | 3;

-- sub optimal but we're trying to preserve written order due to floating point
-- + x = 1 | (2 | 3)
SET x := 1 | (2 | 3);

-- + x = 1 | (3 + 4 | 5);
SET x := 1 | (3 + 4 | 5);

-- + x = 1 | 3 + (4 | 5);
SET x := 1 | 3 + (4 | 5);

-- +  x = (1 | 3) + (4 | 5);
SET x := (1 | 3) + (4 | 5);

-- + x = (1 + 2) * 5;
SET x := (1 + 2) * 5;

-- + x = 1 + 2 - 1;
SET x := (1 + 2) - 1;

-- + x = 1 << 2 | 3;
SET x := 1 << 2 | 3;

-- + x = 1 << (2 | 3);
SET x := 1 << (2 | 3);

-- + x = 1 | 2 << 3
SET x := 1 | (2 << 3);

-- + x = 1 << (2 << 3);
SET x := 1 << (2 << 3);

-- + x = 1 < (2 > 3);
SET x := 1 < (2 > 3);

-- + x = 1 << (2 >> 3);
SET x := 1 << (2 >> 3);

-- + x = 1 | (2 | 3);
SET x := 1 | (2 | 3);

-- + x = 1 | 2 | 3;
SET x := (1 | 2) | 3;

-- + x = 1 == (2 != 3);
SET x := 1 == (2 != 3);

CREATE TABLE SalesInfo(
  month INTEGER,
  amount real
);

-- TEST: ORDERBY BETWEEN PRECEEDING AND FOLLOWING NO FILTER NO EXCLUDE
-- + AVG(amount) OVER (ORDER BY month ROWS BETWEEN 1 PRECEDING AND 1 FOLLOWING) AS SalesMovingAverage
CREATE PROC window1()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (ORDER BY month ROWS BETWEEN 1 PRECEDING AND 1 FOLLOWING)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: simple OVER AND ORDER BY
-- + SUM(amount) OVER (ORDER BY month) AS RunningTotal
CREATE PROC window2()
BEGIN
  SELECT month, amount, SUM(amount) OVER
    (ORDER BY month) RunningTotal
  FROM SalesInfo;
END;

-- TEST: ROWS expr preceeding AND expr following, exclude no others
-- + AVG(amount) OVER (ORDER BY month ROWS BETWEEN 1 PRECEDING AND 2 FOLLOWING EXCLUDE NO OTHERS) AS SalesMovingAverage
CREATE PROC window3()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (ORDER BY month ROWS BETWEEN 1 PRECEDING AND 2 FOLLOWING EXCLUDE NO OTHERS)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: ROWS expr preceeding AND expr following, exclude no others WITH FILTER
-- + AVG(amount) FILTER (WHERE month = 1) OVER (ORDER BY month ROWS BETWEEN 1 PRECEDING AND 2 FOLLOWING EXCLUDE NO OTHERS) AS SalesMovingAverage
CREATE PROC window4()
BEGIN
  SELECT month, amount, AVG(amount) FILTER(WHERE month = 1) OVER
    (ORDER BY month ROWS BETWEEN 1 PRECEDING AND 2 FOLLOWING EXCLUDE NO OTHERS)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: ROWS expr preceeding AND expr following, exclude current row
-- + AVG(amount) OVER (ORDER BY month ROWS BETWEEN 3 PRECEDING AND 4 FOLLOWING EXCLUDE CURRENT ROW) AS SalesMovingAverage
CREATE PROC window5()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (ORDER BY month ROWS BETWEEN 3 PRECEDING AND 4 FOLLOWING EXCLUDE CURRENT ROW)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: ROWS expr preceeding AND expr following, exclude group
-- + AVG(amount) OVER (ORDER BY month ROWS BETWEEN 4 PRECEDING AND 5 FOLLOWING EXCLUDE GROUP) AS SalesMovingAverage
CREATE PROC window6()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (ORDER BY month ROWS BETWEEN 4 PRECEDING AND 5 FOLLOWING EXCLUDE GROUP)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: ROWS expr preceeding AND expr following, exclude ties
-- + AVG(amount) OVER (ORDER BY month ROWS BETWEEN 6 PRECEDING AND 7 FOLLOWING EXCLUDE TIES) AS SalesMovingAverage
CREATE PROC window7()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (ORDER BY month ROWS BETWEEN 6 PRECEDING AND 7 FOLLOWING EXCLUDE TIES)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: RANGE expr preceeding AND expr following, exclude ties
-- + AVG(amount) OVER (ORDER BY month RANGE BETWEEN 8 PRECEDING AND 9 FOLLOWING EXCLUDE TIES) AS SalesMovingAverage
CREATE PROC window8()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (ORDER BY month RANGE BETWEEN 8 PRECEDING AND 9 FOLLOWING EXCLUDE TIES)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: GROUPS expr preceeding AND expr following, exclude ties
-- + AVG(amount) OVER (ORDER BY month GROUPS BETWEEN 10 PRECEDING AND 11 FOLLOWING EXCLUDE TIES) AS SalesMovingAverage
CREATE PROC window9()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (ORDER BY month GROUPS BETWEEN 10 PRECEDING AND 11 FOLLOWING EXCLUDE TIES)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: GROUPS unbounded proceeding AND expr following, exclude ties
-- + AVG(amount) OVER (ORDER BY month GROUPS BETWEEN UNBOUNDED PRECEDING AND 12 FOLLOWING EXCLUDE TIES) AS SalesMovingAverage
CREATE PROC window10()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (ORDER BY month GROUPS BETWEEN UNBOUNDED PRECEDING AND 12 FOLLOWING EXCLUDE TIES)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: GROUPS expr following AND expr preceeding
-- + AVG(amount) OVER (ORDER BY month GROUPS BETWEEN 13 FOLLOWING AND 14 PRECEDING) AS SalesMovingAverage
CREATE PROC window11()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (ORDER BY month GROUPS BETWEEN 13 FOLLOWING AND 14 PRECEDING)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: GROUPS between current row AND unbounded following
-- + AVG(amount) OVER (ORDER BY month GROUPS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING) AS SalesMovingAverage
CREATE PROC window12()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (ORDER BY month GROUPS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: GROUPS between unbounded preceding AND current row WITH no exclude
-- + AVG(amount) OVER (ORDER BY month GROUPS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW) AS SalesMovingAverage
CREATE PROC window13()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (ORDER BY month GROUPS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: GROUPS between unbounded preceding AND current row WITH exclude ties
-- +  AVG(amount) OVER (ORDER BY month GROUPS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW EXCLUDE TIES) AS SalesMovingAverage
CREATE PROC window14()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (ORDER BY month GROUPS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW EXCLUDE TIES)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: correct parse AND re-emit of CURRENT_ROW
-- + AVG(amount) OVER (PARTITION BY month ORDER BY month GROUPS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW EXCLUDE TIES) AS SalesMovingAverage
CREATE PROC window15()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (PARTITION BY month ORDER BY month GROUPS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW EXCLUDE TIES)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: correct parse AND re-emit of CURRENT_ROW
-- + AVG(amount) OVER (GROUPS CURRENT ROW) AS SalesMovingAverage
CREATE PROC window16()
BEGIN
  SELECT month, amount, AVG(amount) OVER
    (GROUPS CURRENT ROW)
  SalesMovingAverage FROM SalesInfo;
END;

-- TEST: use result code in a procedure
-- + DECLARE PROC emit_rc (OUT result_code INTEGER NOT NULL) USING TRANSACTION;
-- + CQL_WARN_UNUSED cql_code emit_rc(sqlite3 *_NonNULL _db_, cql_int32 *_NonNULL result_code)
-- + cql_code _rc_ = SQLITE_OK;
-- + *result_code = SQLITE_OK;
CREATE PROC emit_rc(out result_code INTEGER NOT NULL)
BEGIN
  SET result_code := @rc;
END;

-- TEST: ensure that we use the right result code FOR thrown AND storage
-- this code samples the @rc value at various places, the different names
-- allow us to be sure that we're using the right code in each scope.
-- + cql_code _rc_ = SQLITE_OK;
-- + cql_int32 err = 0;
-- + cql_int32 e0 = 0;
-- + cql_int32 e1 = 0;
-- + cql_int32 e2 = 0;
-- + cql_int32 e3 = 0;
-- + cql_int32 e4 = 0;
-- + cql_int32 e5 = 0;
-- + cql_int32 e6 = 0;
-- + e0 = SQLITE_OK;
-- + err = SQLITE_OK;
-- + int32_t _rc_thrown_1 = _rc_;
-- + err = _rc_thrown_1;
-- + e1 = _rc_thrown_1;
-- + e2 = _rc_thrown_1;
-- + int32_t _rc_thrown_2 = _rc_;
-- + e3 = _rc_thrown_2;
-- + err = _rc_thrown_2;
-- + _rc_ = cql_best_error(_rc_thrown_2);
-- + e4 = _rc_thrown_1;
-- + int32_t _rc_thrown_3 = _rc_;
-- + e5 = _rc_thrown_3;
-- + printf("Error %d\n", err);
-- + e6 = SQLITE_OK;
CREATE PROC rc_test()
BEGIN
  LET err := @rc;
  LET e0 := @rc;
  BEGIN try
  BEGIN try
    CREATE TABLE whatever_anything(id INTEGER);
  end try;
  BEGIN catch
    SET err := @rc;
    LET e1 := @rc;
    BEGIN try
       LET e2 := @rc;
       CREATE TABLE whatever_anything(id INTEGER);
    end try;
    BEGIN catch
       LET e3 := @rc;
       SET err := @rc;
       throw;
    end catch;
    LET e4 := @rc;
  end catch;
  end try;
  BEGIN catch
    LET e5 := @rc;
    call printf("Error %d\n", err);
  end catch;
  LET e6 := @rc;
END;

-- TEST: lazy decl of rcthrown variables (via throw)
-- - int32_t _rc_thrown_1 = _rc_;
-- + int32_t _rc_thrown_2 = _rc_;
-- +  _rc_ = cql_best_error(_rc_thrown_2);
CREATE PROC rc_test_lazy1()
BEGIN
  BEGIN try
    CREATE TABLE whatever_anything(id INTEGER);
  end try;
  BEGIN catch
    BEGIN try
       CREATE TABLE whatever_anything(id INTEGER);
    end try;
    BEGIN catch
       throw;
    end catch;
  end catch;
END;

-- TEST: lazy decl of rcthrown variables (via @rc)
-- - int32_t _rc_thrown_1 = _rc_;
-- + int32_t _rc_thrown_2 = _rc_;
-- +  err = _rc_thrown_2;
CREATE PROC rc_test_lazy2()
BEGIN
  BEGIN try
    CREATE TABLE whatever_anything(id INTEGER);
  end try;
  BEGIN catch
    BEGIN try
       CREATE TABLE whatever_anything(id INTEGER);
    end try;
    BEGIN catch
       LET err := @rc;
    end catch;
  end catch;
END;

-- TEST: make an INTEGER enum
DECLARE enum some_ints INTEGER (
  foo = 12,
  bar = 3
);

-- TEST: make a float enum
DECLARE enum some_reals real (
  foo = 12,
  bar = 3
);

-- TEST: make a long enum
DECLARE enum some_longs long (
  foo = 87363537363847643647937,
  bar = 3
);

-- TEST: force these into the .h file, there will be two copies of some_ints
@emit_enums some_ints;
@emit_enums;

-- TEST: force these into the .h file, there will be two copies of some_longs
@emit_enums some_longs;

-- TEST: resolve a virtual table, note that the arguments become the declaration
-- + "CREATE VIRTUAL TABLE virt_table USING virt_module ( "
-- +   "id INTEGER, "
-- +   "t TEXT)");
CREATE PROC virtual_table_creator()
BEGIN
  -- this will be rewritten
  CREATE virtual TABLE virt_table using virt_module (arguments following) AS (
    id INTEGER,
    t text
  );
END;

-- TEST: the CURSOR here should not have the out arg form of y
-- + C.x = 1;
-- + C.y = 1;
-- + out_arg_cursor(C.x, &C.y);
CREATE PROC out_arg_cursor(x INTEGER NOT NULL, out y INTEGER NOT NULL)
BEGIN
  DECLARE C CURSOR LIKE out_arg_cursor arguments;
  fetch C FROM VALUES(1,1);
  call out_arg_cursor(from C);
END;

-- TEST: CREATE virtual table
-- + "CREATE VIRTUAL TABLE v1 USING m1");
-- + "CREATE VIRTUAL TABLE v2 USING m2 (x)");
-- + "CREATE VIRTUAL TABLE v3 USING m2 ( "
-- +   "id INTEGER)");
CREATE PROC make_virt_table()
BEGIN
  CREATE virtual TABLE v1 using m1 AS (id INTEGER);
  CREATE virtual TABLE v2 using m2(x) AS (id INTEGER);
  CREATE virtual TABLE v3 using m2(arguments following) AS (id INTEGER);
END;

-- TEST: declaration of a named type
DECLARE my_name_type type text NOT NULL;

-- make a virtual TABLE WITH a hidden column FOR use in the next tests
CREATE virtual TABLE virtual_with_hidden using module_name AS (
  vx INTEGER hidden NOT NULL,
  vy INTEGER,
  vz INTEGER hidden NOT NULL
);

-- TEST: hidden applied on virtual tables
-- + "SELECT vy "
-- + "FROM virtual_with_hidden");
CREATE PROC virtual1()
BEGIN
  SELECT * FROM virtual_with_hidden;
END;

-- TEST: hidden columns may be used by name
-- +  _rc_ = cql_prepare(_db_, _result_stmt,
-- + "SELECT vx, vy, vz "
-- + "FROM virtual_with_hidden "
-- + "WHERE vx = 2");
CREATE PROC virtual2()
BEGIN
  SELECT vx, vy, vz FROM virtual_with_hidden WHERE vx = 2;
END;

-- TEST: INSERT INTO the table, verify autoexpand is correct there, too
-- only "y" should be inserted here
-- + _rc_ = cql_exec(_db_,
-- + "INSERT INTO virtual_with_hidden(vy) VALUES(1)");
INSERT INTO virtual_with_hidden VALUES(1);

-- TEST: you can use the hidden column IF you do it by name
-- + _rc_ = cql_exec(_db_,
-- + "INSERT INTO virtual_with_hidden(vx, vy) VALUES(1, 2)");
INSERT INTO virtual_with_hidden(vx, vy) VALUES(1,2);

-- TEST: get row FROM the bar TABLE OR else -1
-- + IF (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
-- + IF (_rc_ == SQLITE_ROW) {
-- +   cql_column_NULLable_int32(_temp_stmt, 0, &_tmp_n_int_1);
-- +   cql_set_NULLable(i0_NULLable, _tmp_n_int_1.is_NULL, _tmp_n_int_1.value);
-- + }
-- + else {
-- +   cql_set_notNULL(i0_NULLable, - 1);
-- + }
SET i0_NULLable := (SELECT type FROM bar IF nothing -1);

-- TEST: normal code gen FOR IF nothing throw
-- + SET i0_NULLable := ( SELECT type
-- + FROM bar IF NOTHING THROW );
-- + _rc_ = cql_prepare(_db_, &_temp_stmt,
-- +   "SELECT type "
-- +     "FROM bar");
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
-- + _rc_ = sqlite3_step(_temp_stmt);
-- + IF (_rc_ != SQLITE_ROW) { cql_error_trace(); goto cql_cleanup; }
SET i0_NULLable := (SELECT type FROM bar IF nothing throw);

-- TEST: get row FROM bar IF no row OR NULL -1
-- + IF (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
-- + IF (_rc_ == SQLITE_ROW) {
-- +   cql_column_NULLable_int32(_temp_stmt, 0, &_tmp_n_int_1);
-- + }
-- + IF (_rc_ == SQLITE_DONE || _tmp_n_int_1.is_NULL) {
-- +   i2 = - 1;
-- + } else {
-- +   i2 = _tmp_n_int_1.value;
-- + }
SET i2 := (SELECT type FROM bar IF nothing OR NULL -1);

-- TEST: get row FROM the bar TABLE OR else ""
-- + IF (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
-- + IF (_rc_ == SQLITE_ROW) {
-- +   cql_column_NULLable_string_ref(_temp_stmt, 0, &_tmp_n_text_1);
-- +   cql_set_string_ref(&t0_NULLable, _tmp_n_text_1);
-- + }
-- + else {
-- +   cql_set_string_ref(&t0_NULLable, _literal_%_);
-- + }
SET t0_NULLable := (SELECT name FROM bar IF nothing "");

-- TEST: get row FROM the bar TABLE OR else "garbonzo"
-- + IF (_rc_ != SQLITE_ROW && _rc_ != SQLITE_DONE) { cql_error_trace(); goto cql_cleanup; }
-- + IF (_rc_ == SQLITE_ROW) {
-- +   cql_column_NULLable_string_ref(_temp_stmt, 0, &_tmp_n_text_1);
-- + }
-- + IF (_rc_ == SQLITE_DONE || !_tmp_n_text_1) {
-- +   cql_set_string_ref(&t2, _literal_%_garbonzo_);
-- + } else {
-- +   cql_set_string_ref(&t2, _tmp_n_text_1);
-- + }
SET t2 := (SELECT name FROM bar IF nothing OR NULL "garbonzo");


-- TEST: verify private exports AND binding
-- + DECLARE PROC private_proc (OUT x INTEGER);
-- + static void private_proc(cql_NULLable_int32 *_NonNULL x)
@attribute(cql:private)
CREATE PROC private_proc(out x INTEGER)
BEGIN
  SET x := 1;
END;

-- TEST: verify that getters are not present on private out union but the fetcher is
-- + .crc = CRC_private_out_union,
-- + CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // a_field
-- + DECLARE PROC private_out_union () OUT UNION (a_field INTEGER NOT NULL);
-- + static void private_out_union_fetch_results(private_out_union_result_set_ref _NULLable *_NonNULL _result_set_) {
-- -- no getter
-- - private_out_union_get_a_field
@attribute(cql:private)
CREATE PROC private_out_union()
BEGIN
  DECLARE C CURSOR LIKE SELECT 1 a_field;

  fetch C FROM VALUES(1);
  out union C;
END;

-- TEST: verify that when alt_prefix is set, alt_prefix is the prefix of emitted FUNCTION name.
-- + void c_proc_with_alt_prefix(cql_NULLable_int32 *_NonNULL x)
-- - void proc_with_alt_prefix(cql_NULLable_int32 *_NonNULL x)
@attribute(cql:alt_prefix=c_)
CREATE PROC proc_with_alt_prefix(out x INTEGER)
BEGIN
  SET x := 1;
END;

-- TEST: use the private out union FUNCTION in the same translation unit, it should have everything we need to call it
-- note that compiling this code in C correctly is part of the test which verifies lots of linkage in addition
-- to just these strings.
-- + private_out_union_fetch_results(&C_result_set_);
CREATE PROC use_private_out_union()
BEGIN
  DECLARE C CURSOR FOR call private_out_union();
  loop fetch C
  BEGIN
    call printf("%d\n", C.a_field);
  END;
END;

-- TEST: verify that getters are not present on no getters out union but the fetcher is
-- + .crc = CRC_no_getters_out_union,
-- + CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // a_field
-- + DECLARE PROC no_getters_out_union () OUT UNION (a_field INTEGER NOT NULL);
-- - static void
-- + void no_getters_out_union_fetch_results(no_getters_out_union_result_set_ref _NULLable *_NonNULL _result_set_) {
-- -- no getter
-- - no_getters_out_union_get_a_field
@attribute(cql:suppress_getters)
CREATE PROC no_getters_out_union()
BEGIN
  DECLARE C CURSOR LIKE SELECT 1 a_field;

  fetch C FROM VALUES(1);
  out union C;
END;

-- TEST: use the private out union FUNCTION in the same translation unit, it should have everything we need to call it
-- note that compiling this code in C correctly is part of the test which verifies lots of linkage in addition
-- to just these strings.
-- + no_getters_out_union_fetch_results(&C_result_set_);
CREATE PROC use_no_getters_out_union()
BEGIN
  DECLARE C CURSOR FOR call no_getters_out_union();
  loop fetch C
  BEGIN
    call printf("%d\n", C.a_field);
  END;
END;

-- TEST: verify that getters are not present on suppress results out union but the fetcher is
-- + .crc = CRC_suppress_results_out_union,
-- + CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL, // a_field
-- + DECLARE PROC suppress_results_out_union () OUT UNION (a_field INTEGER NOT NULL);
-- - static void
-- + void suppress_results_out_union_fetch_results(suppress_results_out_union_result_set_ref _NULLable *_NonNULL _result_set_) {
-- -- no getter
-- - suppress_results_out_union_get_a_field
@attribute(cql:suppress_result_set)
CREATE PROC suppress_results_out_union()
BEGIN
  DECLARE C CURSOR LIKE SELECT 1 a_field;

  fetch C FROM VALUES(1);
  out union C;
END;

-- TEST: use the private out union FUNCTION in the same translation unit, it should have everything we need to call it
-- note that compiling this code in C correctly is part of the test which verifies lots of linkage in addition
-- to just these strings.
-- + suppress_results_out_union_fetch_results(&C_result_set_);
CREATE PROC use_suppress_results_out_union()
BEGIN
  DECLARE C CURSOR FOR call suppress_results_out_union();
  loop fetch C
  BEGIN
    call printf("%d\n", C.a_field);
  END;
END;

-- TEST: verify private exports AND binding FOR result SET case
-- + DECLARE PROC private_result (OUT x INTEGER) (x INTEGER NOT NULL);
-- + static CQL_WARN_UNUSED cql_code private_result(sqlite3 *_NonNULL _db_, sqlite3_stmt *_NULLable *_NonNULL _result_stmt, cql_NULLable_int32 *_NonNULL x) {
-- -- cql_code private_result_fetch_results
@attribute(cql:private)
CREATE PROC private_result(out x INTEGER)
BEGIN
  SELECT 1 x;
END;

-- TEST: private PROC forward ref results in static prototype
-- + static void private_fwd_ref(cql_int32 x);
@attribute(cql:private)
DECLARE PROC private_fwd_ref(x INTEGER NOT NULL);

-- TEST: ensure out args SET to NULL FOR ref types
-- + void set_out_arg_ref_test(cql_string_ref _NULLable *_NonNULL x) {
-- + *(void **)x = NULL; // SET out arg to non-garbage
CREATE PROC set_out_arg_ref_test(out x text)
BEGIN
END;

-- TEST: ensure out args SET to NULL FOR NULLable types
-- + void set_out_arg_NULL_test(cql_NULLable_int32 *_NonNULL x) {
-- + cql_set_NULL(*x); // SET out arg to non-garbage
CREATE PROC set_out_arg_NULL_test(out x INTEGER)
BEGIN
END;

-- TEST: ensure out args SET to NULL FOR non-NULL types
-- + void set_out_arg_notNULL_test(cql_int32 *_NonNULL x) {
-- + *x = 0; // SET out arg to non-garbage
CREATE PROC set_out_arg_notNULL_test(out x INTEGER NOT NULL)
BEGIN
END;

DECLARE global_cursor2 CURSOR LIKE SELECT "x" x;

-- TEST: closing a CURSOR should finalize its statement IF it has one AND values IF it has them
-- + CQL_WARN_UNUSED cql_code early_close_cursor(sqlite3 *_NonNULL _db_) {
-- + cql_finalize_stmt(&global_cursor_stmt);
-- + cql_teardown_row(global_cursor2);
CREATE PROC early_close_cursor()
BEGIN
  close global_cursor;
  close global_cursor2;
END;

-- TEST: construct a lot of variables of various types
-- + cql_double r = 0;
-- + cql_int32 i = 0;
-- + cql_int64 l = 0;
-- + cql_string_ref t = NULL;
-- + cql_NULLable_int64 nl = { .is_NULL = 1 };
-- + cql_NULLable_int32 ni = { .is_NULL = 1 };
-- + cql_NULLable_double nr = { .is_NULL = 1 };
-- + cql_string_ref nt = NULL;
-- + r = 1.0;
-- + i = 1;
-- + l = _64(1);
-- + cql_set_string_ref(&t, _literal_%_T_various_lets);
-- + cql_set_notNULL(nl, (~_64(2)));
-- + cql_set_notNULL(ni, (2 + 2));
-- + cql_set_notNULL(nr, 2.0);
-- + cql_set_string_ref(&nt, _literal_%_NT_various_lets);
-- + sl = (~_64(3));
-- + si = (3 + 3);
-- + sr = 3.0;
-- + cql_set_string_ref(&st, _literal_%_ST_various_lets);
-- - Error
CREATE PROC various_lets()
BEGIN
  LET r := 1.0;
  LET i := 1;
  LET l := 1L;
  LET t := "T";
  LET nl := NULLable(~2L);
  LET ni := NULLable(2+2);
  LET nr := NULLable(2.0);
  LET nt := NULLable("NT");
  LET sl := sensitive(~3L);
  LET si := sensitive(3+3);
  LET sr := sensitive(3.0);
  LET st := sensitive("ST");
END;

-- TEST: check that rc is SET correctly in try/catch blocks
-- +1 cql_code _rc_ = SQLITE_OK;
-- two FOR setting the code plus one FOR the init AS above
-- +2 _rc_ = SQLITE_OK;
CREATE PROC try_catch_rc()
BEGIN
  DECLARE C CURSOR FOR SELECT 'foo' extra2 FROM bar;
  BEGIN try
    fetch C;
  end try;
  BEGIN catch
  end catch;
END;

-- TEST: basic code gen FOR the switch
-- + switch (i2) {
-- + case 1:
-- + case 3:
-- + i2 = 30;
-- four code blocks one each FOR (1,3) AND (4), (5), AND default
-- +4 break;
-- + case 4:
-- + i2 = 40;
-- + default:
-- + i2 = 50;
-- case 5 must be present because there is a default, so it needs the case label AND break;
-- + case 5:
-- + }
switch i2
  when 1, 3 then
    SET i2 := 30;
  when 4 then
    SET i2 := 40;
  when 5 then nothing
  else
    SET i2 := 50;
END;

-- TEST: basic code gen FOR the switch (no default)
-- + switch (i2) {
-- + case 1:
-- + case 3:
-- + i2 = 30;
-- only two code blocks FOR (1,3) AND (4); 5 is omitted, no default
-- +2 break;
-- + case 4:
-- + i2 = 40;
-- - default:
-- case 5 is no longer present because there is no default so we can just omit the label AND save code
-- - case 5:
-- + }
switch i2
  when 1, 3 then
    SET i2 := 30;
  when 4 then
    SET i2 := 40;
  when 5 then nothing
END;

-- TEST: basic code gen FOR the switch (no default, int64)
-- + switch (l2) {
-- + case 1:
-- + case 3:
-- + i2 = 30;
-- +2 break;
-- + case _64(4):
-- + i2 = 40;
-- - default:
-- - case _64(5):
-- + }
switch l2
  when 1, 3 then
    SET i2 := 30;
  when 4L then
    SET i2 := 40;
  when 5 then nothing
END;

-- TEST: special case: just excluding 1, 2, 3... no statements but the ELSE
-- + switch (i2) {
-- + case 1:
-- + case 2:
-- + case 3:
-- two net cases (1,2,3) AND default
-- +2 break;
-- + default:
-- + i2 = 123;
switch i2
  when 1, 2, 3 then nothing
  else
    SET i2 := 123;
END;

-- TEST: use of LEAVE within a switch
-- +  switch (i2) {
-- +    case 1:
-- +      IF (i2) {
-- +        break;
-- +      }
-- +      i2 = 999;
-- +3      break;
-- +    default:
-- +      i2 = 1;
-- +      break;
-- +  }
switch i2
  when 1 then
    IF i2 then leave; end if;
    SET i2 := 999;
  else
    SET i2 := 1;
END;

-- used in the next suite of tests
DECLARE PROC out2_proc(x INTEGER, out y INTEGER NOT NULL, out z INTEGER NOT NULL);

-- TEST: implicit DECLARE including re-use
-- + void out_decl_test(cql_NULLable_int32 x) {
-- + cql_int32 u = 0;
-- + cql_int32 v = 0;
-- +2 out2_proc(x, &u, &v);
CREATE PROC out_decl_test(x INTEGER)
BEGIN
  DECLARE out call out2_proc(x, u, v);
  DECLARE out call out2_proc(x, u, v);
END;

-- TEST: implicit DECLARE within a loop; this is a different case because
-- sem_declare_out_call_stmt has to take care to retain the SEM_TYPE_IMPLICIT
-- flags appropriately during loop reanalysis
-- + void out_decl_loop_test(cql_NULLable_int32 x) {
-- + cql_int32 u = 0;
-- + cql_int32 v = 0;
-- +2 out2_proc(x, &u, &v);
CREATE PROC out_decl_loop_test(x INTEGER)
BEGIN
  WHILE 1
  BEGIN
    DECLARE out call out2_proc(x, u, v);
    DECLARE out call out2_proc(x, u, v);
  END;
END;

-- TEST: most binary operations involving a NULL-typed argument result in NULL
-- + cql_set_NULL(add0);
-- + cql_set_NULL(add1);
-- + cql_set_NULL(bin_and0);
-- + cql_set_NULL(bin_and1);
-- + cql_set_NULL(bin_or0);
-- + cql_set_NULL(bin_or1);
-- + cql_set_NULL(div0);
-- + cql_set_NULL(div1);
-- + cql_set_NULL(ge0);
-- + cql_set_NULL(ge1);
-- + cql_set_NULL(gt0);
-- + cql_set_NULL(gt1);
-- + cql_set_NULL(le0);
-- + cql_set_NULL(le1);
-- + cql_set_NULL(like0);
-- + cql_set_NULL(like1);
-- + cql_set_NULL(lshift0);
-- + cql_set_NULL(lshift1);
-- + cql_set_NULL(lt0);
-- + cql_set_NULL(lt1);
-- + cql_set_NULL(mod0);
-- + cql_set_NULL(mod1);
-- + cql_set_NULL(mul0);
-- + cql_set_NULL(mul1);
-- + cql_set_NULL(not_like0);
-- + cql_set_NULL(not_like1);
-- + cql_set_NULL(rshift0);
-- + cql_set_NULL(rshift1);
-- + cql_set_NULL(sub0);
-- + cql_set_NULL(sub1);
-- - cql_set_notNULL
-- - Error
CREATE PROC binary_ops_with_NULL()
BEGIN
  LET add0 := NULL + 42;
  LET add1 := 42 + NULL;
  LET bin_and0 := NULL & 42;
  LET bin_and1 := 42 & NULL;
  LET bin_or0 := NULL | 42;
  LET bin_or1 := 42 | NULL;
  LET div0 := NULL / 42;
  LET div1 := 42 / NULL;
  LET ge0 := NULL >= 42;
  LET ge1 := 42 >= NULL;
  LET gt0 := NULL > 42;
  LET gt1 := 42 > NULL;
  LET le0 := NULL <= 42;
  LET le1 := 42 <= NULL;
  LET like0 := NULL LIKE "foo";
  LET like1 := "foo" LIKE NULL;
  LET lshift0 := NULL << 42;
  LET lshift1 := 42 << NULL;
  LET lt0 := NULL < 42;
  LET lt1 := 42 < NULL;
  LET mod0 := NULL % 42;
  LET mod1 := 42 % NULL;
  LET mul0 := NULL * 42;
  LET mul1 := 42 * NULL;
  LET not_like0 := NULL NOT LIKE "foo";
  LET not_like1 := "foo" NOT LIKE NULL;
  LET rshift0 := NULL >> 42;
  LET rshift1 := 42 >> NULL;
  LET sub0 := NULL - 42;
  LET sub1 := 42 - NULL;
END;

-- Verify that this is a DML PROC even though it does nothing but use throw
-- + DECLARE PROC uses_throw () USING TRANSACTION;
-- + CQL_WARN_UNUSED cql_code uses_throw(sqlite3 *_NonNULL _db_) {
-- + _rc_ = cql_best_error(SQLITE_OK);
-- + cql_error_trace();
CREATE PROC uses_throw()
BEGIN
  throw;
END;

-- TEST: verify that this is a DML PROC even though it does nothing but ifNULL_throw
-- + DECLARE PROC uses_ifNULL_throw (x INTEGER) USING TRANSACTION;
-- + CQL_WARN_UNUSED cql_code uses_ifNULL_throw(sqlite3 *_NonNULL _db_, cql_NULLable_int32 x) {
-- + _rc_ = SQLITE_ERROR;
-- + cql_error_trace();
CREATE PROC uses_ifNULL_throw(x int)
BEGIN
   LET y := ifNULL_throw(x);
END;

-- +  CQL_DATA_TYPE_OBJECT | CQL_DATA_TYPE_NOT_NULL, // o
-- + cql_offsetof(out_object_row, o)
CREATE PROC out_object(o object NOT NULL)
BEGIN
  DECLARE C CURSOR LIKE out_object arguments;
  fetch C FROM arguments;
  out C;
END;

-- TEST: Verify that contracts are inserted WHERE appropriate (and not inserted
-- WHERE not appropriate)
-- + cql_contract_argument_notNULL((void *)d, 4);
-- + cql_contract_argument_notNULL((void *)f, 6);
-- + cql_contract_argument_notNULL((void *)h, 8);
-- + cql_contract_argument_notNULL((void *)i, 9);
-- + cql_contract_argument_notNULL((void *)j, 10);
-- + cql_contract_argument_notNULL((void *)k, 11);
-- + cql_contract_argument_notNULL((void *)l, 12);
-- + cql_contract_argument_notNULL((void *)m, 13);
-- + cql_contract_argument_notNULL((void *)n, 14);
-- + cql_contract_argument_notNULL((void *)o, 15);
-- + cql_contract_argument_notNULL_when_dereferenced((void *)p, 16);
-- +11 cql_contract_argument_notNULL
-- +1 cql_contract_argument_notNULL_when_dereferenced
CREATE PROC exercise_contracts(
  a int,
  b int NOT NULL,
  c text,
  d text NOT NULL,
  e blob,
  f blob NOT NULL,
  g object,
  h object NOT NULL,
  out i int,
  out j int NOT NULL,
  out k text,
  out l text NOT NULL,
  inout m int,
  inout n int NOT NULL,
  inout o text,
  inout p text NOT NULL,
)
BEGIN
  SET l := "text";
END;

-- TEST: Contracts should be emitted FOR public procs
-- + cql_contract_argument_notNULL((void *)t, 1);
CREATE PROC public_proc_with_a_contract(t text NOT NULL)
BEGIN
END;

-- TEST: Contracts should not be emitted FOR private procs
-- - cql_contract_argument_notNULL((void *)t, 1);
@attribute(cql:private)
CREATE PROC private_proc_without_a_contract(t text NOT NULL)
BEGIN
END;

-- TEST: Contracts should be emitted only in _fetch_results FOR result SET procs
-- +1 cql_contract_argument_notNULL((void *)t, 1);
CREATE PROC result_set_proc_with_contract_in_fetch_results(t text NOT NULL)
BEGIN
  SELECT * FROM bar;
END;

-- TEST: Contracts should be emitted only in _fetch_results FOR out procs
-- +1 cql_contract_argument_notNULL((void *)t, 1);
CREATE PROC out_proc_with_contract_in_fetch_results(t text NOT NULL)
BEGIN
  DECLARE C CURSOR LIKE bar;
  out C;
END;

-- TEST: The improving of NULLable variables compiles to nothing in SQL.
-- + "SELECT ? + 1"
CREATE PROC NULLability_improvements_are_erased_for_sql()
BEGIN
  DECLARE a int;
  IF a is NOT NULL then
    SELECT (a + 1) AS b;
  end if;
END;

-- TEST: The improving of NULLable variables to be nonNULL respects the
-- underlying NULLable representation.
-- + cql_NULLable_int32 a = { .is_NULL = 1 };
-- + cql_int32 b = 0;
-- + b = a.value;
-- + cql_set_notNULL(a, 0);
CREATE PROC NULLability_improvements_do_not_change_access()
BEGIN
  DECLARE a int;
  IF a is NOT NULL then
    LET b := a;
    SET a := 0;
  end if;
END;

-- TEST: a loose SELECT statement generates no code (and will produce no errors)
-- the errors are checked when this code is compiled in C.  IF the code
-- were generated there would be errors because the global proc
-- doesn't have the statement out arg.  We also verify that
-- no call to cql_prepare happens hence no select
-- - cql_prepare
SELECT 1 x;

-- TEST: we should infer a bool NOT NULL variable AND compute is true correctly
-- + true_test = !!(1);
LET true_test := 1 is true;

-- TEST: we should infer a bool NOT NULL variable AND compute is false correctly
-- + false_test = !(0);
LET false_test := 0 is false;

-- TEST: we should infer a bool type AND use the NULLable version of the test
-- + true_test = cql_is_NULLable_true(i0_NULLable.is_NULL, i0_NULLable.value);
SET true_test := i0_NULLable is true;

-- TEST: we should infer a bool type AND use the NULLable version of the test
-- + false_test = cql_is_NULLable_false(i0_NULLable.is_NULL, i0_NULLable.value);
SET false_test := i0_NULLable is false;

-- TEST: we should infer a bool NOT NULL variable AND compute is true correctly
-- + true_test = !(1);
SET true_test := 1 is not true;

-- TEST: we should infer a bool NOT NULL variable AND compute is false correctly
-- + false_test = !!(0);
SET false_test := 0 is not false;

-- TEST: we should infer a bool type AND use the NULLable version of the test
-- + true_test = !cql_is_NULLable_true(i0_NULLable.is_NULL, i0_NULLable.value);
SET true_test := i0_NULLable is not true;

-- TEST: we should infer a bool type AND use the NULLable version of the test
-- + false_test = !cql_is_NULLable_false(i0_NULLable.is_NULL, i0_NULLable.value);
SET false_test := i0_NULLable is not false;

CREATE TABLE big_data(
  f1 LONG_INT NOT NULL,
  f2 INTEGER NOT NULL,
  f3 TEXT,
  f4 TEXT NOT NULL,
  f5 TEXT,
  f6 TEXT,
  f7 LONG_INT,
  f8 LONG_INT NOT NULL,
  f9 LONG_INT NOT NULL,
  f10 LONG_INT NOT NULL,
  f11 LONG_INT NOT NULL,
  f12 TEXT @SENSITIVE,
  f13 BOOL NOT NULL,
  f14 LONG_INT,
  f15 BOOL,
  f16 INTEGER NOT NULL,
  f17 INTEGER NOT NULL,
  f18 TEXT,
  f19 INTEGER,
  f20 TEXT,
  f21 INTEGER,
  f22 TEXT,
  f23 INTEGER,
  f24 LONG_INT NOT NULL,
  f25 TEXT,
  f26 BOOL NOT NULL,
  f27 BOOL NOT NULL,
  f28 BOOL NOT NULL,
  f29 TEXT,
  f30 TEXT,
  f31 TEXT,
  f32 INTEGER,
  f33 LONG_INT,
  f34 INTEGER,
  f35 TEXT,
  f36 TEXT,
  f38 LONG_INT NOT NULL,
  f39 LONG_INT UNIQUE,
  f40 BOOL,
  f41 BOOL NOT NULL,
  f42 TEXT,
  f43 TEXT,
  f44 LONG_INT,
  f45 BOOL NOT NULL,
  f46 LONG_INT,
  f47 INTEGER NOT NULL,
  f48 TEXT,
  f49 LONG_INT,
  f50 TEXT,
  f51 TEXT,
  f52 LONG_INT,
  f53 INTEGER NOT NULL,
  f54 TEXT,
  f55 LONG_INT NOT NULL,
  f56 LONG_INT NOT NULL,
  f57 TEXT,
  f58 TEXT,
  f59 INTEGER,
  f60 TEXT,
  f61 INTEGER,
  f62 LONG_INT,
  f63 LONG_INT,
  f64 INTEGER,
  f65 LONG_INT NOT NULL,
  f66 INTEGER NOT NULL,
  f67 INTEGER NOT NULL,
  f68 INTEGER,
  f69 TEXT,
  f70 REAL,
  f71 LONG_INT,
  f72 INTEGER,
  f73 INTEGER,
  f74 LONG_INT,
  f75 INTEGER
);

-- TEST: big test needs not string temporaries just one helper call
-- we do not want to see the "get" pattern
-- - cql_set_string_ref(&s, cql_cursor_format(&C_dyn));
-- we want to see the "create" pattern (i.e. we start WITH a +1 ref)
-- + cql_string_release(s);
-- + s = cql_cursor_format(&C_dyn);
-- - cql_string_release(_tmp_text
CREATE PROC BigFormat ()
BEGIN
  DECLARE C CURSOR FOR SELECT * FROM big_data;
  LOOP FETCH C
  BEGIN
    LET s := cql_cursor_format(C);
  END;
END;

-- TEST: codegen FOR sign
-- + _tmp_int_2 = - 2;
-- + sign_val_int = ((_tmp_int_2 > 0) - (_tmp_int_2 < 0));
LET sign_val_int := sign(-2);

-- TEST: codegen FOR sign: NULLable arg
-- + cql_set_notNULL(_tmp_n_int_2, (-2));
-- + cql_set_NULLable(sign_val_NULLable, _tmp_n_int_2.is_NULL, ((_tmp_n_int_2.value > 0) - (_tmp_n_int_2.value < 0)));
LET sign_val_NULLable := sign(NULLable(-2));

-- TEST: codegen FOR absolute value
-- + _tmp_int_2 = - 2;
-- + abs_val_int = abs(_tmp_int_2);
LET abs_val_int := abs(-2);

-- TEST: codegen FOR absolute value: NULLable arg
-- + cql_set_notNULL(_tmp_n_int_2, (-2));
-- + cql_set_NULLable(abs_val_NULLable, _tmp_n_int_2.is_NULL, abs(_tmp_n_int_2.value));
LET abs_val_NULLable := abs(NULLable(-2));

-- TEST: codegen FOR absolute value long
-- +  _tmp_int64_2 = - _64(2);
-- +  abs_val_long = labs(_tmp_int64_2);
LET abs_val_long := abs(-2L);

-- TEST: codegen FOR absolute value real
-- + _tmp_double_2 = - 2.0;
-- + abs_val_real = fabs(_tmp_double_2);
LET abs_val_real := abs(-2.0);

-- TEST: codegen FOR absolute value bool
-- + _tmp_bool_% = 1;
-- + abs_val_bool = !!_tmp_bool_%;
LET abs_val_bool := abs(true);

-- TEST: codegen FOR absolute value of NULL
-- + cql_set_NULL(abs_val_NULLable);
SET abs_val_NULLable := abs(NULL);


-- Used in the following test.
CREATE PROC ltor_proc_int_not_NULL(a int NOT NULL, b int NOT NULL, out c int NOT NULL) BEGIN END;
CREATE PROC ltor_proc_int(a int, b int, out c int) BEGIN END;
CREATE PROC ltor_proc_text_not_NULL(a text NOT NULL, b text NOT NULL, out c text NOT NULL) BEGIN SET c := "text"; END;
CREATE PROC ltor_proc_text(a text, b text, out c text) BEGIN END;
DECLARE FUNCTION ltor_func_int_not_NULL(a int NOT NULL, b int NOT NULL) int NOT NULL;
DECLARE FUNCTION ltor_func_int(a int, b int) int;
DECLARE FUNCTION ltor_func_text_not_NULL(a text NOT NULL, b text NOT NULL) text NOT NULL;
DECLARE FUNCTION ltor_func_text(a text, b text) text;

-- TEST: Arguments are always evaluated left-to-right (which is ensured by
-- generating temps).
-- + ltor_proc_int_not_NULL(1, 2, &_tmp_int_%);
-- + ltor_proc_int_not_NULL(3, 4, &_tmp_int_%);
-- + ltor_proc_int_not_NULL(_tmp_int_%, _tmp_int_%, &a);
-- + ltor_proc_int(_tmp_n_int_%, _tmp_n_int_%, &_tmp_n_int_%);
-- + ltor_proc_int(_tmp_n_int_%, _tmp_n_int_%, &_tmp_n_int_%);
-- + ltor_proc_int(_tmp_n_int_%, _tmp_n_int_%, &b);
-- + ltor_proc_text_not_NULL(_literal_%_arg%, _literal_%_arg%, &_tmp_text_%);
-- + ltor_proc_text_not_NULL(_literal_%_arg%, _literal_%_arg%, &_tmp_text_%);
-- + ltor_proc_text_not_NULL(_tmp_text_%, _tmp_text_%, &c);
-- + ltor_proc_text(_literal_%_arg%, _literal_%_arg%, &_tmp_n_text_%);
-- + ltor_proc_text(_literal_%_arg%, _literal_%_arg%, &_tmp_n_text_%);
-- + ltor_proc_text(_tmp_n_text_%, _tmp_n_text_%, &d);
-- + _tmp_int_% = ltor_func_int_not_NULL(1, 2);
-- + _tmp_int_% = ltor_func_int_not_NULL(3, 4);
-- + e = ltor_func_int_not_NULL(_tmp_int_%, _tmp_int_%);
-- + _tmp_n_int_% = ltor_func_int(_tmp_n_int_%, _tmp_n_int_%);
-- + _tmp_n_int_% = ltor_func_int(_tmp_n_int_%, _tmp_n_int_%);
-- + f = ltor_func_int(_tmp_n_int_%, _tmp_n_int_%);
-- + cql_set_string_ref(&_tmp_text_%, ltor_func_text_not_NULL(_literal_%_arg%, _literal_%_arg%));
-- + cql_set_string_ref(&_tmp_text_%, ltor_func_text_not_NULL(_literal_%_arg%, _literal_%_arg%));
-- + cql_set_string_ref(&g, ltor_func_text_not_NULL(_tmp_text_%, _tmp_text_%));
-- + cql_set_string_ref(&_tmp_n_text_%, ltor_func_text(_literal_%_arg%, _literal_%_arg%));
-- + cql_set_string_ref(&_tmp_n_text_%, ltor_func_text(_literal_%_arg%, _literal_%_arg%));
-- + cql_set_string_ref(&h, ltor_func_text(_tmp_n_text_%, _tmp_n_text_%));
CREATE PROC arguments_are_evaluated_left_to_right()
BEGIN
  LET a := ltor_proc_int_not_NULL(ltor_proc_int_not_NULL(1, 2), ltor_proc_int_not_NULL(3, 4));
  LET b := ltor_proc_int(ltor_proc_int(1, 2), ltor_proc_int(3, 4));
  LET c := ltor_proc_text_not_NULL(ltor_proc_text_not_NULL("1", "2"), ltor_proc_text_not_NULL("3", "4"));
  LET d := ltor_proc_text(ltor_proc_text("1", "2"), ltor_proc_text("3", "4"));
  LET e := ltor_func_int_not_NULL(ltor_func_int_not_NULL(1, 2), ltor_func_int_not_NULL(3, 4));
  LET f := ltor_func_int(ltor_func_int(1, 2), ltor_func_int(3, 4));
  LET g := ltor_func_text_not_NULL(ltor_func_text_not_NULL("1", "2"), ltor_func_text_not_NULL("3", "4"));
  LET h := ltor_func_text(ltor_func_text("1", "2"), ltor_func_text("3", "4"));
END;

CREATE PROC f1(out x INTEGER NOT NULL)
BEGIN
  SET x := 5;
END;

CREATE PROC f2(out x INTEGER )
BEGIN
  SET x := 5;
END;

CREATE PROC f3(y INTEGER, out x INTEGER )
BEGIN
  SET x := y;
END;

-- TEST: ensure that the temporary FROM calling f1 is not reused in the 3rd call
-- this was previously problematic because in the case of f1 we have a NOT NULL
-- result so no result variable is used, the expression FOR the first f1() + f1()
-- becomes something LIKE tmp1 + tmp2 but then that part of the AST returns
-- AND we reused tmp1 again FOR the next call resulting in tmp1 + tmp2 + tmp1
-- which is very bad indeed
--- NOT NULL CASE: NO TEMPS CAN BE REUSED!
-- +  f1(&_tmp_int_2);
-- +  f1(&_tmp_int_3);
-- +  f1(&_tmp_int_4);
-- +  q = _tmp_int_2 + _tmp_int_3 + _tmp_int_4;
--- NULLABLE CASE: TEMPS CAN BE REUSED tmp1 combines tmp2 AND tmp3, tmp2 can be reused!
-- +  f2(&_tmp_n_int_2);
-- +  f2(&_tmp_n_int_3);
-- +  cql_combine_NULLables(_tmp_n_int_1, _tmp_n_int_2.is_NULL, _tmp_n_int_3.is_NULL, _tmp_n_int_2.value + _tmp_n_int_3.value);
-- +  f2(&_tmp_n_int_2);
-- +  cql_combine_NULLables(r, _tmp_n_int_1.is_NULL, _tmp_n_int_2.is_NULL, _tmp_n_int_1.value + _tmp_n_int_2.value);
--- NULLABLE CASE WITH BOXING: TEMPS CAN BE REUSED tmp1 combines tmp2 AND tmp3, both can be reused
-- +  cql_set_notNULL(_tmp_n_int_3, 0);
-- +  f3(_tmp_n_int_3, &_tmp_n_int_2);
-- +  cql_set_notNULL(_tmp_n_int_4, 1);
-- +  f3(_tmp_n_int_4, &_tmp_n_int_3);
-- +  cql_combine_NULLables(_tmp_n_int_1, _tmp_n_int_2.is_NULL, _tmp_n_int_3.is_NULL, _tmp_n_int_2.value + _tmp_n_int_3.value);
-- +  cql_set_notNULL(_tmp_n_int_3, 2);
-- +  f3(_tmp_n_int_3, &_tmp_n_int_2);
-- + cql_combine_NULLables(s, _tmp_n_int_1.is_NULL, _tmp_n_int_2.is_NULL, _tmp_n_int_1.value + _tmp_n_int_2.value);
CREATE PROC multi_call_temp_reuse()
BEGIN
  LET q := f1() + f1() + f1();
  LET r := f2() + f2() + f2();
  LET s := f3(0) + f3(1) + f3(2);
END;

-- TEST: The `sensitive` FUNCTION is a no-op AND never appears in the C output.
-- + cql_string_ref x = NULL;
-- + cql_set_string_ref(&x, _literal_%_hello_sensitive_function_is_a_no_op);
-- + _rc_ = cql_prepare(_db_, _result_stmt,
-- + "SELECT 'hello'");
CREATE PROC sensitive_function_is_a_no_op()
BEGIN
  LET x := sensitive("hello");
  SELECT sensitive("hello") AS y;
END;

-- TEST: the AND operator has unusual short circuit evaluation
-- we had a bug WHERE the right arg was evaluated first but
-- emitted second, this caused it to clobber temps FROM the left
-- this test verifies that the temps inside the OR are correct
-- tmp_bool_1, _2, AND _3 must be distinct.
-- IF you evaluate in the in the wrong order you end up WITH overlap
-- this is a big snip but it's necessary here
-- +  cql_combine_NULLables(_tmp_n_bool_1, a.is_NULL, b.is_NULL, a.value > b.value);
-- +  IF (cql_is_NULLable_false(_tmp_n_bool_1.is_NULL, _tmp_n_bool_1.value)) {
-- +    cql_set_notNULL(_tmp_n_bool_0, 0);
-- +  }
-- +  else {
-- +    cql_combine_NULLables(_tmp_n_bool_3, a.is_NULL, c.is_NULL, a.value < c.value);
-- +    IF (cql_is_NULLable_true(_tmp_n_bool_3.is_NULL, _tmp_n_bool_3.value)) {
-- +      cql_set_notNULL(_tmp_n_bool_2, 1);
-- +    }
-- +    else {
-- +      IF (c.is_NULL) {
-- +        cql_set_notNULL(_tmp_n_bool_2, 1);
-- +      }
-- +      else {
-- +        cql_set_NULLable(_tmp_n_bool_2, _tmp_n_bool_3.is_NULL, 0);
-- +      }
-- +    }
-- +    IF (cql_is_NULLable_false(_tmp_n_bool_2.is_NULL, _tmp_n_bool_2.value)) {
-- +      cql_set_notNULL(_tmp_n_bool_0, 0);
-- +    }
-- +    else {
-- +      cql_combine_NULLables(_tmp_n_bool_0, _tmp_n_bool_1.is_NULL, _tmp_n_bool_2.is_NULL, 1);
-- +    }
-- +  }
-- +  IF (cql_is_NULLable_true(_tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value)) {
-- +    cql_set_NULLable(c, a.is_NULL, a.value);
-- +  }
CREATE PROC and_preserves_temps(a long, b long, c long)
BEGIN
  IF a > b AND (a < c OR c is NULL) then
     SET c := a;
  end if;
END;

-- TEST: the OR operator has unusual short circuit evaluation
-- we had a bug WHERE the right arg was evaluated first but
-- emitted second, this caused it to clobber temps FROM the left
-- this test verifies that the temps inside the OR are correct.
-- tmp_bool_1, _2, AND _3 must be distinct.
-- this is a big snip but it's necessary here
-- IF you evaluate in the in the wrong order you end up WITH overlap
-- +  cql_set_NULLable(_tmp_n_bool_1, c.is_NULL, c.value < 0);
-- +  IF (cql_is_NULLable_true(_tmp_n_bool_1.is_NULL, _tmp_n_bool_1.value)) {
-- +    cql_set_notNULL(_tmp_n_bool_0, 1);
-- +  }
-- +  else {
-- +    cql_combine_NULLables(_tmp_n_bool_3, a.is_NULL, c.is_NULL, a.value > c.value);
-- +    IF (cql_is_NULLable_false(_tmp_n_bool_3.is_NULL, _tmp_n_bool_3.value)) {
-- +      cql_set_notNULL(_tmp_n_bool_2, 0);
-- +    }
-- +    else {
-- +      cql_combine_NULLables(_tmp_n_bool_4, b.is_NULL, c.is_NULL, b.value > c.value);
-- +      IF (cql_is_NULLable_false(_tmp_n_bool_4.is_NULL, _tmp_n_bool_4.value)) {
-- +        cql_set_notNULL(_tmp_n_bool_2, 0);
-- +      }
-- +      else {
-- +        cql_combine_NULLables(_tmp_n_bool_2, _tmp_n_bool_3.is_NULL, _tmp_n_bool_4.is_NULL, 1);
-- +      }
-- +    }
-- +    IF (cql_is_NULLable_true(_tmp_n_bool_2.is_NULL, _tmp_n_bool_2.value)) {
-- +      cql_set_notNULL(_tmp_n_bool_0, 1);
-- +    }
-- +    else {
-- +      cql_combine_NULLables(_tmp_n_bool_0, _tmp_n_bool_1.is_NULL, _tmp_n_bool_2.is_NULL, 0);
-- +    }
-- +  }
-- +  IF (cql_is_NULLable_true(_tmp_n_bool_0.is_NULL, _tmp_n_bool_0.value)) {
-- +    cql_set_NULLable(c, a.is_NULL, a.value);
-- +  }
CREATE PROC or_preserves_temps(a long, b long, c long)
BEGIN
  IF c < 0 OR (a > c AND b > c) then
     SET c := a;
  end if;
END;

-- TEST: make sure we don't emit this into the output
-- - cql_code
@attribute(cql:shared_fragment)
CREATE PROC shared_frag()
BEGIN
 SELECT 1234 shared_something; -- hence no cql_code return type
END;

-- TEST use the above
-- note that the generated string has the query parts above
-- Fragment sandwich:
-- ---- first we see the prepare_var variant
-- +  _rc_ = cql_prepare_var(_db_, _result_stmt,
-- --- three parts in this sandwich
-- +    3,
-- +  "WITH "
-- +    "shared_frag (shared_something) AS (",
-- ---- then we see the shared fragment-- note the name can be elided AND it is!
-- +  "SELECT 1234",
-- ---- then we see what came after the shared fragment
-- +  ") "
-- +    "SELECT shared_something "
-- +      "FROM shared_frag"
-- +  );
-- CREATE PROC foo()
-- BEGIN
--   with
--     (call shared_frag())
--   SELECT * FROM shared_frag;
-- END;

-- used in the following test
@attribute(cql:shared_fragment)
CREATE PROC shared_conditional(x INTEGER NOT NULL)
BEGIN
  IF x == 1 then
    SELECT x AS x;
  else IF x == 2 then
    SELECT x + x AS x;
  else
    SELECT x + x + x AS x;
  end if;
END;

-- TEST: use a conditional shared fragment in various ways
-- this PROC forces a variable sandwich, there are
-- variables before the conditionals inside the conditonals
-- (see above) AND after the conditionals
-- 5 text fragments
-- + char _preds_1[5];
-- 8 variable usages
-- + char _vpreds_1[8];
-- + memset(&_preds_1[0], 0, sizeof(_preds_1));
-- + memset(&_vpreds_1[0], 0, sizeof(_vpreds_1));
-- control flow to figure out which predicates to enable
-- + _p1_x_ = 1;
-- + _preds_1[0] = 1;
-- + _vpreds_1[0] = 1; // pred 0 known to be 1
-- + IF (_p1_x_ == 1) {
-- +   _preds_1[1] = 1;
-- +   _vpreds_1[1] = 1; // pred 1 known to be 1
-- + }
-- + else {
-- +   IF (_p1_x_ == 2) {
-- +     _preds_1[2] = 1;
-- +     _vpreds_1[2] = 1; // pred 2 known to be 1
-- +     _vpreds_1[3] = 1; // pred 2 known to be 1
-- +   }
-- +   else {
-- +     _preds_1[3] = 1;
-- +     _vpreds_1[4] = 1; // pred 3 known to be 1
-- +     _vpreds_1[5] = 1; // pred 3 known to be 1
-- +     _vpreds_1[6] = 1; // pred 3 known to be 1
-- +   }
-- + }
-- + _preds_1[4] = 1;
-- + _vpreds_1[7] = 1; // pred 0 known to be 1
-- + _rc_ = cql_prepare_var(_db_, _result_stmt,
-- + 5, _preds_1,
--
-- root fragment 0 always present
-- + "WITH "
-- +   "some_cte (id) AS (SELECT ?), "
-- +   "shared_conditional (x) AS (",
--
-- option 1 fragment 1
-- + "SELECT ?",
--
-- option 2 fragment 2
-- + "SELECT ? + ?",
--
-- option 3 fragment 3
-- + "SELECT ? + ? + ?",
--
-- pop to root, fragment 4 condition same AS fragment 0
-- + ") "
-- +   "SELECT bar.id, bar.name, bar.rate, bar.type, bar.size "
-- +     "FROM bar "
-- +     "INNER JOIN some_cte ON ? = 5"
--
-- 8 variable sites, only some of which are used
-- + cql_multibind_var(&_rc_, _db_, _result_stmt, 8, _vpreds_1,
-- CREATE PROC shared_conditional_user(x INTEGER NOT NULL)
-- BEGIN
--   with
--   some_cte(id) AS (SELECT x),
--   (call shared_conditional(1))
--   SELECT bar.* FROM bar join some_cte on x = 5;
-- END;

-- used in the following test, this is silly fragment
-- but it forces complex push AND pop of variable state
-- @attribute(cql:shared_fragment)
-- CREATE PROC nested_shared_proc(x_ INTEGER NOT NULL)
-- BEGIN
--   IF x_ <= 5 then
--     with
--     (call shared_conditional(1))
--     SELECT * FROM shared_conditional WHERE x_ == 5;
--   else
--     SELECT x_ AS x;
--   end if;
-- END;

-- TEST: variable arg management in a nested context
-- + memset(&_preds_1[0], 0, sizeof(_preds_1));
-- + memset(&_vpreds_1[0], 0, sizeof(_vpreds_1));
-- + _p1_x__ = 1;
-- + _preds_1[0] = 1;
-- + IF (_p1_x__ <= 5) {
-- +   _preds_1[1] = 1;
-- +   _p2_x_ = 1;
-- +   IF (_p2_x_ == 1) {
-- +     _preds_1[2] = 1;
-- +     _vpreds_1[0] = 1; // pred 2 known to be 1
-- +   }
-- +   else {
-- +     IF (_p2_x_ == 2) {
-- +       _preds_1[3] = 1;
-- +       _vpreds_1[1] = 1; // pred 3 known to be 1
-- +       _vpreds_1[2] = 1; // pred 3 known to be 1
-- +     }
-- +     else {
-- +       _preds_1[4] = 1;
-- +       _vpreds_1[3] = 1; // pred 4 known to be 1
-- +       _vpreds_1[4] = 1; // pred 4 known to be 1
-- +       _vpreds_1[5] = 1; // pred 4 known to be 1
-- +     }
-- +   }
-- this is what's unique about this test, we popped back to the context of predicate 1
-- +   _preds_1[5] = _preds_1[1];
-- +   _vpreds_1[6] = _preds_1[1];
-- + }
-- + else {
-- +   _preds_1[6] = 1;
-- +   _vpreds_1[7] = 1; // pred 6 known to be 1
-- + }
-- CREATE PROC nested_shared_stuff()
-- BEGIN
--   with
--   (call nested_shared_proc(1))
--   SELECT * FROM nested_shared_proc;
-- END;

-- TEST: nested SELECT syntax WITH complex fragment
--
-- 10 fragments AND 8 variables AS expected
-- control flow corresponds to the nested selects (manually verified)
-- see discussion per fragment
-- +  char _preds_1[10];
-- +  char _vpreds_1[8];
-- +  memset(&_preds_1[0], 0, sizeof(_preds_1));
-- +  memset(&_vpreds_1[0], 0, sizeof(_vpreds_1));
-- +  _p1_x__ = 1;
-- +  _preds_1[0] = 1;
-- +  _preds_1[1] = 1;
-- +  IF (_p1_x__ <= 5) {
-- +    _preds_1[2] = 1;
-- +    _p2_x_ = 1;
-- +    IF (_p2_x_ == 1) {
-- +      _preds_1[3] = 1;
-- +      _vpreds_1[0] = 1; // pred 3 known to be 1
-- +    }
-- +    else {
-- +      IF (_p2_x_ == 2) {
-- +        _preds_1[4] = 1;
-- +        _vpreds_1[1] = 1; // pred 4 known to be 1
-- +        _vpreds_1[2] = 1; // pred 4 known to be 1
-- +      }
-- +      else {
-- +        _preds_1[5] = 1;
-- +        _vpreds_1[3] = 1; // pred 5 known to be 1
-- +        _vpreds_1[4] = 1; // pred 5 known to be 1
-- +        _vpreds_1[5] = 1; // pred 5 known to be 1
-- +      }
-- +    }
-- +    _preds_1[6] = _preds_1[2];
-- +    _vpreds_1[6] = _preds_1[2];
-- +  }
-- +  else {
-- +    _preds_1[7] = 1;
-- +    _vpreds_1[7] = 1; // pred 7 known to be 1
-- +  }
-- +  _preds_1[8] = 1;
-- +  _preds_1[9] = 1;
-- +  _rc_ = cql_prepare_var(_db_, _result_stmt,
-- +    10, _preds_1,
--
-- fragment 0 always present
-- +  "SELECT x "
-- +      "FROM (",
--
-- fragment 1, the nested wrapper -- always present
-- +  "WITH _ns_(x) AS (",
--
-- fragment 2 present IF x <= 5
-- +  "WITH "
-- +    "shared_conditional (x) AS (",
--
-- fragment 3 present IF x == 1
-- first variable binding v[0] = pred[3]
-- +  "SELECT ?",
--
-- fragment 4 present IF x == 2
-- second variable binding v[1], v[2] = pred[4]
-- +  "SELECT ? + ?",
--
-- fragment 5 present IF x == 3
-- third variable binding v[3], v[4], v[5] = pred[5]
-- +  "SELECT ? + ? + ?",
--
-- fragment 6 the tail of fragment 2, present IF x <= 5
-- fourth variable binding v[6] = pred[6] = pred[2]
-- +  ") "
-- +    "SELECT x "
-- +      "FROM shared_conditional "
-- +      "WHERE ? = 5",
--
-- fragment 7 present IF x > 5
-- fifth variable binding v[7] = pred[7] = !pred[2]
-- +  "SELECT ?",
--
-- fragment 8 present always
-- +  ") SELECT * FROM _ns_",
--
-- fragment 9 present always
-- +  ")"
-- CREATE PROC use_nested_select_shared_frag_form()
-- BEGIN
--   SELECT * FROM (call nested_shared_proc(1));
-- END;

-- TEST: in the nested SELECT case we have to wrap the fragment text WITH a CTE
-- the column names are needed AND the CTE does not provide them.  The _ns_ wrapper
-- accomplishes this.  We do it this way so that the text of the fragment is the same
-- IF we are using nested SELECT OR not.
-- + "SELECT shared_something "
-- + "FROM (",
-- + "WITH _ns_(shared_something) AS (",
-- + "SELECT 1234",
-- + ") SELECT * FROM _ns_",
-- + ")"
-- @attribute(cql:private)
-- CREATE PROC simple_shared_frag()
-- BEGIN
--   SELECT * FROM (call shared_frag());
-- END;


-- used in the next test
-- @attribute(cql:shared_fragment)
-- CREATE PROC shared_frag_else_nothing(id_ INTEGER)
-- BEGIN
--   IF id_ > 0 then
--     SELECT id_ AS id1, 'x' AS text1;
--   else
--     SELECT nothing;
--   end if;
-- END;

-- TEST: SELECT nothing expands into the right number of columns
-- + "SELECT 0,0 WHERE 0",
-- CREATE PROC shared_frag_else_nothing_test()
-- BEGIN
--   WITH (call shared_frag_else_nothing(5))
--   SELECT * FROM foo;
-- END;

-- DECLARE const group some_constants (
--   const_u = false,
--   const_w = 3.5,
--   const_x = 1L,
--   const_y = 2+3,
--   const_z = "hello, world\n"
-- );

-- TEST: slash star AND star slash safety
-- when we generate the comment FOR this PROC we have to
-- nix the slash star AND star slash OR otherwise things will fail
-- in the generated C code:
--
--   - FOR star slash, not doing so will result in the comment
--     block ending prematurely, resulting in invalid C code that
--     won't compile.
--   - FOR slash star, there is a high chance that the compiler
--     will reject the generated code under certain configurations
--     (-Werror AND -Wcomment flags).
CREATE PROC slash_star_and_star_slash()
BEGIN
  LET x := "/*  */";
END;

-- @emit_constants some_constants;

@attribute(cql:blob_storage)
CREATE TABLE structured_storage(
  id INTEGER NOT NULL,
  name text NOT NULL
);

-- TEST: basic blob serialization case
-- + _rc_ = cql_serialize_to_blob(&B, &C_dyn);
-- + _rc_ = cql_deserialize_from_blob(B, &D_dyn);
CREATE PROC blob_serialization_test()
BEGIN
  DECLARE C CURSOR FOR SELECT 1 id, 'foo' name;
  fetch C;

  DECLARE B blob<structured_storage>;

  SET B FROM CURSOR C;

  DECLARE D CURSOR LIKE C;
  fetch D FROM B;
END;

DECLARE FUNCTION make_blob() CREATE blob<structured_storage>;

-- TEST: get a blob FROM somewhere other than a local
-- checks general expression evaluation in the fetch path
-- func call is a good standing FOR general eval
-- +  cql_blob_release(_tmp_n_blob_0);
-- + _tmp_n_blob_0 = make_blob();
-- + _rc_ = cql_deserialize_from_blob(_tmp_n_blob_0, &C_dyn);
CREATE PROC deserialize_func()
BEGIN
  DECLARE C CURSOR LIKE structured_storage;
  -- fetch C FROM blob make_blob();
  fetch C FROM make_blob();
END;

-- TEST: ensure that the max constants are getting handled correctly
-- including the special cases to avoid compiler warnings.  Note that
-- this code has to compile correctly in C to pass the test also.  Run
-- time checks FOR this are in run_test.sql because this is subtle
--
-- +  big1 = _64(0x7fffffffffffffff);
-- +  big2 = _64(0x8000000000000000);
-- +  big3 = (_64(-9223372036854775807) - 1);
-- +  big4 = (_64(-9223372036854775807) - 1);
-- +  big5 = _64(9223372036854775807);
-- +  big6 = _64(9223372036854775807);
CREATE PROC bigstuff()
BEGIN
  LET big1 := 0x7fffffffffffffffL;
  LET big2 := 0x8000000000000000L;
  LET big3 := -9223372036854775808L;
  LET big4 := -9223372036854775808;
  LET big5 := 9223372036854775807L;
  LET big6 := 9223372036854775807;
END;

-- DECLARE const group big_constants(
--   big_long_constants_max = 9223372036854775807,
--   big_long_constants_min = -9223372036854775808,
--   big_long_constants_almost_min = -9223372036854775807
-- );

-- @emit_constants big_constants;

-- TEST: variable group creates declarations only
-- group produces nothing in the main stream!
-- - struct
-- - row
-- - define
-- - extern
-- - error:
-- DECLARE group var_group
-- BEGIN
--   DECLARE gr_cursor CURSOR LIKE SELECT 1 x, "2" y;
--   DECLARE gr_integer INTEGER;
--   DECLARE gr_blob_cursor CURSOR LIKE structured_storage;
-- END;

-- TEST: emits the definitions only
-- + gr_cursor_row gr_cursor = { ._refs_count_ = 1, ._refs_offset_ = gr_cursor_refs_offSET };
-- + cql_NULLable_int32 gr_integer = { .is_NULL = 1 };
--
-- additional stuff FOR a CURSOR that needs to be serialized
-- note that the arrays are not static FOR a global cursor
-- + gr_blob_cursor_row gr_blob_cursor = { ._refs_count_ = 1, ._refs_offset_ = gr_blob_cursor_refs_offSET };
-- + cql_uint16 gr_blob_cursor_cols[] = { 2,
-- +   cql_offsetof(gr_blob_cursor_row, id),
-- +   cql_offsetof(gr_blob_cursor_row, name)
-- + };
-- + uint8_t gr_blob_cursor_data_types[] = {
-- +   CQL_DATA_TYPE_INT32 | CQL_DATA_TYPE_NOT_NULL,
-- +   CQL_DATA_TYPE_STRING | CQL_DATA_TYPE_NOT_NULL
-- + };
-- @emit_group var_group;

-- TEST: use the global CURSOR FOR serialization
-- This sets the SERIALIZATION bit on the CURSOR causing it to emit more stuff
-- even though it's out of order the codegen will be affected
-- the test cases above verify this
-- + _rc_ = cql_serialize_to_blob(b, &gr_blob_cursor_dyn);
CREATE PROC use_gr_cursor_for_serialization(out b blob<structured_storage>)
BEGIN
  SET b FROM CURSOR gr_blob_cursor;
END;

-- TEST: IF we mutate a reference arg then we have to track its lifetime
-- we cannot just borrow the reference, the parameter is not released
-- nor can it be...  So convert this to a normal local pattern.
-- + DECLARE PROC mutated_in_param_ref (x TEXT);
-- + void mutated_in_param_ref(cql_string_ref _NULLable _in__x) {
-- + cql_string_ref x = NULL;
-- + cql_set_string_ref(&x, _in__x);
-- + cql_set_string_ref(&x, _literal_%_hi_mutated_in_param_ref);
-- + cql_string_release(x);
CREATE PROC mutated_in_param_ref(x text)
BEGIN
  SET x := 'hi';
END;

-- TEST: likely() is correctly emitted
-- +  _rc_ = cql_prepare(_db_, &_temp_stmt,
-- + "SELECT likely(1)");
-- + IF (_rc_ != SQLITE_OK) { cql_error_trace(); goto cql_cleanup; }
SET b2 := ( SELECT likely(1) );

-- TEST: DECLARE an OUT PROC that will be redeclared AND then created to ensure
-- that struct redeclarations are appropriately guarded against (and, thus, the
-- result can be compiled successfully).
DECLARE PROC some_redeclared_out_proc() out (x int) using transaction;
DECLARE PROC some_redeclared_out_proc() out (x int) using transaction;
CREATE PROC some_redeclared_out_proc()
BEGIN
  DECLARE c CURSOR FOR SELECT NULLable(1) x;
  fetch c;
  out c;
END;

-- TEST: DECLARE an OUT UNION PROC that will be redeclared AND then created to
-- ensure that struct redeclarations are appropriately guarded against (and,
-- thus, the result can be compiled successfully).
DECLARE PROC some_redeclared_out_union_proc() out union (x int) using transaction;
DECLARE PROC some_redeclared_out_union_proc() out union (x int) using transaction;
CREATE PROC some_redeclared_out_union_proc()
BEGIN
  DECLARE c CURSOR FOR SELECT NULLable(1) x;
  fetch c;
  out union c;
END;

-- DECLARE FUNCTION external_cursor_func(x cursor) INTEGER;

-- TEST call a FUNCTION that takes a generic cursor
-- + cql_dynamic_cursor shape_storage_dyn = {
-- + .cursor_data = (void *)&shape_storage,
-- + .cursor_has_row = (void *)&shape_storage._has_row_,
-- + .cursor_data_types = shape_storage_data_types,
-- + .cursor_col_offsets = shape_storage_cols,
-- + result = external_cursor_func(&shape_storage_dyn);
CREATE PROC external_cursor_caller ()
BEGIN
  DECLARE shape_storage CURSOR LIKE SELECT 1 AS x;
  LET result := external_cursor_func(shape_storage);
END;

-- helper method that clobbers x (in out)
CREATE PROC clobber1(inout x text)
BEGIN
  SET x := "xyzzy";
END;

-- helper method that clobbers x (out arg)
CREATE PROC clobber2(out x text)
BEGIN
  SET x := "xyzzy";
END;

-- TEST: use of in arg at in/out position requires copy
-- + void mutated_in_arg1(cql_string_ref _NULLable _in__x) {
-- + cql_string_ref x = NULL;
-- + cql_set_string_ref(&x, _in__x);
CREATE PROC mutated_in_arg1(x text)
BEGIN
  call clobber1(x);
END;

-- TEST: use of in arg at out position requires copy
-- + void mutated_in_arg2(cql_string_ref _NULLable _in__x) {
-- + cql_string_ref x = NULL;
-- + cql_set_string_ref(&x, _in__x);
CREATE PROC mutated_in_arg2(x text)
BEGIN
  call clobber2(x);
END;

-- TEST: use of in arg FOR fetch into requires copy
-- + CQL_WARN_UNUSED cql_code mutated_in_arg3(sqlite3 *_NonNULL _db_, cql_string_ref _NULLable _in__x) {
-- + cql_string_ref x = NULL;
-- + cql_set_string_ref(&x, _in__x);
CREATE PROC mutated_in_arg3(x text)
BEGIN
  DECLARE C CURSOR FOR SELECT "x" x;
  fetch C into x;
END;

-- TEST: make sure the NOT NULL contract is renamed
-- + void mutated_not_NULL(cql_string_ref _NonNULL _in__x) {
-- + cql_contract_argument_notNULL((void *)_in__x, 1);
CREATE PROC mutated_not_NULL(x text NOT NULL)
BEGIN
  SET x := 'xyzzy';
END;

-- TEST: declaration of an unchecked SELECT function
-- DECLARE SELECT FUNCTION no_check_select_fun NO CHECK text;

-- TEST: declaration of an unchecked table-valued SELECT function
-- DECLARE SELECT FUNCTION no_check_select_table_valued_fun NO CHECK (t text);

-- a PROC that returns a value, we will use its shape below
DECLARE PROC a_proc_we_need() (id INTEGER, t text);

-- TEST make sure we export everything we need including the recursive dependency
-- +2 DECLARE PROC a_proc_we_need () (id INTEGER, t TEXT);
-- DECLARE PROC a_proc_that_needs_dependents () OUT UNION (a_foo OBJECT<a_proc_we_need SET>, another_foo OBJECT<a_proc_we_need SET>) USING TRANSACTION;
-- CREATE PROC a_proc_that_needs_dependents()
-- BEGIN
--   DECLARE C CURSOR LIKE (a_foo object<a_proc_we_need set>, another_foo object<a_proc_we_need set>);
--   fetch C using a_proc_we_need() a_foo, a_proc_we_need() another_foo;
--   out union C;
-- END;

-- TEST: check FOR needed types in the args
-- + DECLARE PROC a_proc_we_need () (id INTEGER, t TEXT);
-- + DECLARE PROC another_proc_that_needs_dependents (a_foo OBJECT<a_proc_we_need SET>);
-- CREATE PROC another_proc_that_needs_dependents(a_foo object<a_proc_we_need set>)
-- BEGIN
-- END;


CREATE PROC simple_child_proc()
BEGIN
  SELECT 1 x, 2 y;
END;

-- TEST: emit getters AND setters FOR a simple result SET SET type
-- + cql_bool simple_container_proc_get_a_is_NULL(simple_container_proc_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + return data[row].a.is_NULL;
-- + cql_int32 simple_container_proc_get_a_value(simple_container_proc_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + return data[row].a.value;
-- + extern void simple_container_proc_set_a_value(simple_container_proc_result_set_ref _NonNULL result_set, cql_int32 row, cql_int32 new_value) {
-- + cql_result_set_set_int32_col((cql_result_set_ref)result_set, row, 0, new_value);
-- + extern void simple_container_proc_set_a_to_NULL(simple_container_proc_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + cql_result_set_set_to_NULL_col((cql_result_set_ref)result_set, row, 0);
-- + cql_int32 simple_container_proc_get_b(simple_container_proc_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + return data[row].b;
-- + void simple_container_proc_set_b(simple_container_proc_result_set_ref _NonNULL result_set, cql_int32 row, cql_int32 new_value) {
-- + cql_result_set_set_int32_col((cql_result_set_ref)result_set, row, 1, new_value);
-- + simple_child_proc_result_set_ref _NULLable simple_container_proc_get_c(simple_container_proc_result_set_ref _NonNULL result_set, cql_int32 row) {
-- + return (simple_child_proc_result_set_ref _NULLable )data[row].c;
-- + extern void simple_container_proc_set_c(simple_container_proc_result_set_ref _NonNULL result_set, cql_int32 row, simple_child_proc_result_set_ref _NULLable new_value) {
-- + cql_result_set_set_object_col((cql_result_set_ref)result_set, row, 2, (cql_object_ref)new_value);
-- @attribute(cql:emit_setters)
-- CREATE PROC simple_container_proc()
-- BEGIN
--   DECLARE C CURSOR LIKE (a INTEGER, b INTEGER NOT NULL, c object<simple_child_proc set>);
--   fetch C using
--      1 a,
--      2 b,
--      simple_child_proc() c;

--   out union C;
-- END;

-- these are largely no-op directives until we generate SQL FOR them
-- at this point we just make sure we can generate these without crashing

-- @blob_get_key_type bgetkey_type;
-- @blob_get_val_type bgetval_type;
-- @blob_get_key bgetkey offset;
-- @blob_get_val bgetval;
-- @blob_create_key bcreatekey offset;
-- @blob_create_val bcreateval;
-- @blob_update_key bupdatekey offset;
-- @blob_update_val bupdateval;

@attribute(cql:backing_table)
CREATE TABLE backing(
  k blob primary key,
  v blob
);

@attribute(cql:backed_by=backing)
CREATE TABLE backed(
  flag bool NOT NULL,
  id long,
  name text,
  age real,
  storage blob,

  -- pk not at position 0 tests column index array computation
  pk INT PRIMARY KEY
);

@attribute(cql:backed_by=backing)
CREATE TABLE backed2(
  pk1 int,
  pk2 int,
  flag bool NOT NULL,
  id long,
  name text,
  extra int,
  primary key(pk2, pk1) -- offsets reversed
);

-- TEST: cql_blob_get should expand to the correct calls AND hash codes
-- + "SELECT bgetkey(k, 0), bgetval(v, 1055660242183705531), bgetval(v, -7635294210585028660), bgetval(v, -9155171551243524439), bgetval(v, -6946718245010482247), bgetval(v, -3683705396192132539) "
CREATE PROC use_cql_blob_get_backed()
BEGIN
  DECLARE C CURSOR FOR select
    cql_blob_get(k, backed.pk),
    cql_blob_get(v, backed.flag),
    cql_blob_get(v, backed.storage),
    cql_blob_get(v, backed.id),
    cql_blob_get(v, backed.name),
    cql_blob_get(v, backed.age) FROM backing;
END;

-- TEST: cql_blob_get should expand to the correct calls AND hash codes
-- + "SELECT bgetkey(k, 1), bgetkey(k, 0), bgetval(v, -9155171551243524439), bgetval(v, 4605090824299507084), bgetval(v, -6946718245010482247) "
CREATE PROC use_cql_blob_get_backed2()
BEGIN
  DECLARE C CURSOR FOR select
    cql_blob_get(k, backed2.pk1), -- offsets reversed
    cql_blob_get(k, backed2.pk2),
    cql_blob_get(v, backed2.id),
    cql_blob_get(v, backed2.extra),
    cql_blob_get(v, backed2.name) FROM backing;
END;

-- TEST INSERT INTO backed2 -- keys should be the correct offsets
-- + INSERT INTO backing(k, v)
-- + SELECT bcreatekey(3942979045122214775, V.pk2, 1, V.pk1, 1),
-- + bcreateval(3942979045122214775, 1055660242183705531, V.flag, 0, -9155171551243524439, V.id, 2, -6946718245010482247, V.name, 4, 4605090824299507084, V.extra, 1)
CREATE PROC insert_into_backed2()
BEGIN
  INSERT INTO backed2 VALUES(1, 2, true, 1000, 'hi', 5);
END;

-- TEST UPDATE backed2 -- keys should be the correct offsets
-- note offsets not in the normal order (1, 0)
-- + "SET k = bupdatekey(k, 1, 5, 0, 7) "
CREATE PROC update_backed2()
BEGIN
  UPDATE backed2 SET pk1 = 5, pk2 = 7 WHERE pk1 = 3 AND pk2 = 11;
END;

-- TEST: we should have created a shared fragment called _backed
-- + _backed (rowid, flag, id, name, age, storage, pk) AS (CALL _backed())
-- + SELECT rowid,
-- + bgetkey(T.k, 0)
-- + bgetval(T.v, 1055660242183705531),
-- + bgetval(T.v, -9155171551243524439),
-- + bgetval(T.v, -6946718245010482247),
-- + bgetval(T.v, -3683705396192132539),
-- + bgetval(T.v, -7635294210585028660
-- + FROM backing AS T
-- + SELECT rowid, flag, id, name, age, storage, pk
-- + FROM _backed
-- + WHERE bgetkey_type(T.k) = -5417664364642960231
-- CREATE PROC use_generated_fragment()
-- BEGIN
--   WITH (call _backed())
--   SELECT * FROM _backed;
-- END;

-- TEST: we swap in the shared fragment AND get the columns FROM it
-- + backed (rowid, flag, id, name, age, storage, pk) AS (CALL _backed())
-- + SELECT rowid,
-- + bgetkey(T.k, 0)
-- + bgetval(T.v, 1055660242183705531),
-- + bgetval(T.v, -9155171551243524439),
-- + bgetval(T.v, -6946718245010482247),
-- + bgetval(T.v, -3683705396192132539),
-- + bgetval(T.v, -7635294210585028660
-- + FROM backing AS T
-- + SELECT rowid, flag, id, name, age, storage, pk
-- + FROM backed
-- + WHERE bgetkey_type(T.k) = -5417664364642960231
CREATE PROC use_backed_table_directly()
BEGIN
  SELECT * FROM backed;
END;

-- TEST: we swap in the shared fragment AND get the columns FROM it
-- + backed (rowid, flag, id, name, age, storage, pk) AS (CALL _backed())
-- + SELECT rowid,
-- + bgetkey(T.k, 0)
-- + bgetval(T.v, 1055660242183705531),
-- + bgetval(T.v, -9155171551243524439),
-- + bgetval(T.v, -6946718245010482247),
-- + bgetval(T.v, -3683705396192132539),
-- + bgetval(T.v, -7635294210585028660
-- + FROM backing AS T
-- + SELECT rowid, flag, id, name, age, storage, pk
-- + FROM backed
-- + WHERE bgetkey_type(T.k) = -5417664364642960231
-- verify this is a NOT result SET proc
-- - sqlite3_stmt *_NULLable *_NonNULL _result_stmt
CREATE PROC use_backed_table_with_cursor()
BEGIN
  DECLARE C CURSOR FOR SELECT * FROM backed;
END;

-- TEST: we swap in the shared fragment AND get the columns FROM it
-- + one (x) AS (SELECT 1),
-- + two (x) AS (SELECT 2)
-- + backed (rowid, flag, id, name, age, storage, pk) AS (CALL _backed())
-- + SELECT rowid,
-- + bgetkey(T.k, 0)
-- + bgetval(T.v, 1055660242183705531),
-- + bgetval(T.v, -9155171551243524439),
-- + bgetval(T.v, -6946718245010482247),
-- + bgetval(T.v, -3683705396192132539),
-- + bgetval(T.v, -7635294210585028660
-- + FROM backing AS T
-- + SELECT rowid, flag, id, name, age, storage, pk
-- + FROM backed
-- + WHERE bgetkey_type(T.k) = -5417664364642960231
-- verify this is a result SET proc
-- + sqlite3_stmt *_NULLable *_NonNULL _result_stmt
CREATE PROC use_backed_table_directly_in_with_select()
BEGIN
  WITH one(*) AS (SELECT 1 x), two(*) AS (SELECT 2 x)
  SELECT * FROM backed;
END;

-- TEST: we swap in the shared fragment AND get the columns FROM it
-- + one (x) AS (SELECT 1),
-- + two (x) AS (SELECT 2)
-- + backed (rowid, flag, id, name, age, storage, pk) AS (CALL _backed())
-- + SELECT rowid
-- + bgetkey(T.k, 0)
-- + bgetval(T.v, 1055660242183705531),
-- + bgetval(T.v, -9155171551243524439),
-- + bgetval(T.v, -6946718245010482247),
-- + bgetval(T.v, -3683705396192132539),
-- + bgetval(T.v, -7635294210585028660
-- + FROM backing AS T
-- + SELECT rowid, flag, id, name, age, storage, pk
-- + FROM backed
-- + WHERE bgetkey_type(T.k) = -5417664364642960231
-- verify this is NOT a result SET proc
-- - sqlite3_stmt *_NULLable *_NonNULL _result_stmt
CREATE PROC use_backed_table_with_select_and_cursor()
BEGIN
  DECLARE C CURSOR for
  WITH one(*) AS (SELECT 1 x), two(*) AS (SELECT 2 x)
  SELECT * FROM backed;
END;

-- TEST: SELECT expression WITH backed table
-- + backed (rowid, flag, id, name, age, storage, pk) AS (CALL _backed())
-- + SELECT rowid
-- + bgetkey(T.k, 0)
-- + bgetval(T.v, 1055660242183705531),
-- + bgetval(T.v, -9155171551243524439),
-- + bgetval(T.v, -6946718245010482247),
-- + bgetval(T.v, -3683705396192132539),
-- + bgetval(T.v, -7635294210585028660
-- + FROM backing AS T
-- + SELECT flag
-- + FROM backed
-- + WHERE bgetkey_type(T.k) = -5417664364642960231
-- verify this is NOT a result SET proc
-- - sqlite3_stmt *_NULLable *_NonNULL _result_stmt
CREATE PROC use_backed_table_select_expr(out x bool NOT NULL)
BEGIN
  SET x := (SELECT flag FROM backed);
END;

-- TEST: explain query plan WITH replacement
-- + EXPLAIN QUERY PLAN
-- + backed (rowid, flag, id, name, age, storage, pk) AS (CALL _backed())
-- + SELECT rowid
-- + bgetkey(T.k, 0)
-- + bgetval(T.v, 1055660242183705531),
-- + bgetval(T.v, -9155171551243524439),
-- + bgetval(T.v, -6946718245010482247),
-- + bgetval(T.v, -3683705396192132539),
-- + bgetval(T.v, -7635294210585028660
-- + FROM backing AS T
-- + FROM backed
-- + WHERE bgetkey_type(T.k) = -5417664364642960231
-- verify this is a result SET proc
-- + sqlite3_stmt *_NULLable *_NonNULL _result_stmt
@attribute(cql:private)
CREATE PROC explain_query_plan_backed(out x bool NOT NULL)
BEGIN
  explain query plan SELECT * FROM backed;
END;

-- try the path WHERE we use offsets in the value blob
-- @blob_get_val bgetval offset;

-- TEST: we should get value indexes 0, 1, 2, 3, 4 not hashes
-- + SELECT rowid, bgetval(T.v, 0), bgetval(T.v, 1), bgetval(T.v, 2), bgetval(T.v, 3), bgetval(T.v, 4), bgetkey(T.k, 0)
CREATE PROC use_backed_table_select_expr_value_offsets(out x bool NOT NULL)
BEGIN
  SET x := (SELECT flag FROM backed);
END;

-- go back to the other way
-- @blob_get_val bgetval;

@attribute(cql:backed_by=backing)
CREATE TABLE small_backed(
  pk INT PRIMARY KEY,
  x text,
  y real
);

-- TEST: simple insert WITH values
-- + _vals (pk, x, y) AS (VALUES(1, '2', 3.14), (4, '5', 6), (7, '8', 9.7))
-- + INSERT INTO backing(k, v) SELECT
-- + bcreatekey(-4190907309554122430, V.pk, 1),
-- + bcreateval(-4190907309554122430, 7953209610392031882, V.x, 4, 3032304244189539277, V.y, 3)
-- + FROM _vals AS V
CREATE PROC insert_backed_VALUES()
BEGIN
  INSERT INTO small_backed VALUES(1, "2", 3.14),  (4, "5", 6),  (7, "8", 9.7);
END;

-- TEST: simple with-insert using values
-- + U (x, y, z) AS (VALUES(1, '2', 3.14))
-- + V (x, y, z) AS (VALUES(1, '2', 3.14))
-- + _vals (pk, x, y) AS (SELECT x, y, z
-- + FROM V)
-- + INSERT INTO backing(k, v) SELECT
-- + bcreatekey(-4190907309554122430, V.pk, 1)
-- + bcreateval(-4190907309554122430, 7953209610392031882, V.x, 4, 3032304244189539277, V.y, 3) "
-- + FROM _vals AS V
CREATE PROC insert_backed_values_using_with()
BEGIN
  with
    U(x,y,z) AS (values (1, "2", 3.14)), -- just here to verify that we can keep many CTES
    V(x,y,z) AS (values (1, "2", 3.14))
  INSERT INTO small_backed SELECT * FROM V;
END;

-- TEST: simple insert using form
-- + _vals (pk, x, y) AS (VALUES(1, '2', 3.14))
-- + INSERT INTO backing(k, v) SELECT
-- + bcreatekey(-4190907309554122430, V.pk, 1)
-- + bcreateval(-4190907309554122430, 7953209610392031882, V.x, 4, 3032304244189539277, V.y, 3)
-- + FROM _vals AS V
CREATE PROC insert_backed_values_using_form()
BEGIN
  INSERT INTO small_backed using 1 pk, "2" x, 3.14 y;
END;

-- TEST: insert FROM a select
-- + small_backed (rowid, pk, x, y) AS (
-- + SELECT rowid, bgetkey(T.k, 0) AS pk, bgetval(T.v, 7953209610392031882) AS x, bgetval(T.v, 3032304244189539277) AS y
-- + FROM backing AS T
-- + WHERE bgetkey_type(T.k) = -4190907309554122430
-- + _vals (pk, x, y) AS (SELECT pk + 1000, B.x || 'x', B.y + 50
-- + FROM small_backed AS B)
-- + INSERT INTO backing(k, v) SELECT bcreatekey(-4190907309554122430, V.pk, 1), bcreateval(-4190907309554122430, 7953209610392031882, V.x, 4, 3032304244189539277, V.y, 3)
-- + bcreatekey(-4190907309554122430, V.pk, 1)
-- + bcreateval(-4190907309554122430, 7953209610392031882, V.x, 4, 3032304244189539277, V.y, 3) "
-- + FROM _vals AS V
CREATE PROC inserted_backed_from_select()
BEGIN
  INSERT INTO small_backed SELECT pk+1000, B.x||'x', B.y+50 FROM small_backed B;
END;

-- TEST: DELETE FROM backed
-- + small_backed (rowid, pk, x, y)
-- + DELETE FROM backing WHERE rowid IN (SELECT rowid
-- + FROM small_backed
-- + WHERE pk = 12345)
CREATE PROC delete_from_backed()
BEGIN
  DELETE FROM small_backed WHERE pk = 12345;
END;

-- TEST: DELETE FROM backed WITH no WHERE clause
-- + small_backed (rowid, pk, x, y)
-- + DELETE FROM backing WHERE rowid IN (SELECT rowid
-- + FROM small_backed)
-- + v (x) AS (VALUES(1)
CREATE PROC delete_from_backed_no_where_clause()
BEGIN
  WITH v(x) AS (VALUES(1)) -- force the WITH SELECT form
  DELETE FROM small_backed;
END;

-- TEST: use cql_blob_UPDATE AND validate hash codes etc.
-- + SELECT bupdatekey(?, 0, 1)
-- + SELECT bupdateval(?, -3683705396192132539, 21, 3, -6946718245010482247, 'dave', 4)
CREATE PROC test_blob_update_expand()
BEGIN
  DECLARE b blob;
  LET x := (SELECT cql_blob_update(b, 1, backed.pk));
  LET z := (SELECT cql_blob_update(b, 21, backed.age, "dave", backed.name));
END;


-- TEST: simple UPDATE into backed TABLE value only
-- + UPDATE backing
-- + SET v = bupdateval(v, -6946718245010482247, 'foo', 4)
-- + WHERE rowid IN (SELECT rowid
-- + FROM backed
-- + WHERE name = 'one')
CREATE PROC update_backed_set_value()
BEGIN
  UPDATE backed SET name = 'foo' WHERE name = 'one';
END;

-- TEST: simple UPDATE into backed TABLE value only, using WITH clause
-- + V (x) AS (VALUES(1))
-- + UPDATE backing
-- + SET v = bupdateval(v, -6946718245010482247, 'goo', 4)
-- + WHERE rowid IN (SELECT rowid
-- + FROM backed
-- + WHERE name = 'with_update')
CREATE PROC update_backed_with_clause()
BEGIN
  WITH V(x) AS (VALUES(1)) -- force a WITH clause
  UPDATE backed SET name = 'goo' WHERE name = 'with_update';
END;

-- TEST: simple UPDATE into backed TABLE key only
-- + UPDATE backing
-- + SET k = bupdatekey(k, 0, 100)
-- + WHERE rowid IN (SELECT rowid
-- + FROM backed
-- + WHERE name = 'two')
CREATE PROC update_backed_set_key()
BEGIN
  UPDATE backed SET pk = 100  WHERE name = 'two';
END;

-- TEST: UPDATE key AND value, add other clauses
-- + UPDATE backing
-- + SET k = bupdatekey(k, 0, 100)
-- + v = bupdateval(v, -3683705396192132539, 77, 3)
-- + WHERE rowid IN (SELECT rowid
-- + FROM backed
-- + WHERE name = 'three'
-- + ORDER BY age
-- + LIMIT 7)
CREATE PROC update_backed_set_both()
BEGIN
  UPDATE backed SET pk = 100, age = 77 WHERE name = 'three' ORDER BY age LIMIT 7;
END;

-- TEST: in_loop variation of statement prep WITH DML
-- temp statement is reSET in the loop
-- +1 sqlite3_reset(_temp1_stmt);
-- +1 cql_finalize_stmt(&_temp1_stmt);
-- +2 cql_finalize_stmt(&_temp_stmt);
-- verify that we went back to _temp_stmt after using _temp1_stmt
-- +1 _rc_ = cql_prepare(_db_, &_temp1_stmt,
-- +1 _rc_ = cql_prepare(_db_, &_temp_stmt,
CREATE PROC stmt_in_loop()
BEGIN
   LET i := 0;
   WHILE i < 10
   BEGIN
      DELETE FROM foo WHERE id = i;
      SET i := i + 1;
   END;
   -- verify we go back to the normal temp statement
   LET x := (SELECT EXISTS(SELECT 1 FROM foo));
END;

-- TEST: in_loop variation of statement prep WITH cursor
-- finalize on entry causes another finalize
-- +2 cql_finalize_stmt(&C_stmt);
-- +2 cql_finalize_stmt(&_temp_stmt);
-- verify that we went back to _temp_stmt after using C_stmt
-- +1 _rc_ = cql_prepare(_db_, &C_stmt,
-- +1 _rc_ = cql_prepare(_db_, &_temp_stmt,
CREATE PROC cursor_in_loop()
BEGIN
   LET i := 0;
   WHILE i < 10
   BEGIN
      -- CURSOR C FOR SELECT * FROM foo WHERE id = i;
      fetch C;
      SET i := i + 1;
   END;
   -- verify we go back to the normal temp statement
   LET x := (SELECT EXISTS(SELECT 1 FROM foo));
END;

-- TEST: in_loop variation of SELECT expression case
-- +1 sqlite3_reset(_temp1_stmt);
-- +1 cql_finalize_stmt(&_temp1_stmt);
CREATE PROC select_in_loop()
BEGIN
   WHILE 1
   BEGIN
     LET z := (SELECT 1 z);
   END;
END;

-- TEST: in_loop variation of SELECT expression IF nothing case
-- +1 sqlite3_reset(_temp1_stmt);
-- +1 cql_finalize_stmt(&_temp1_stmt);
CREATE PROC select_if_nothing_in_loop()
BEGIN
   WHILE 1
   BEGIN
     LET z := (SELECT 1 z IF nothing 1);
   END;
END;

-- TEST: in_loop variation of SELECT expression IF nothing OR NULL case
-- +1 sqlite3_reset(_temp1_stmt);
-- +1 cql_finalize_stmt(&_temp1_stmt);
CREATE PROC select_if_nothing_or_NULL_in_loop()
BEGIN
   WHILE 1
   BEGIN
     LET z := (SELECT 1 z IF nothing OR NULL 1);
   END;
END;

-- TEST: test type_check is trip AND the expression is kept in c codegen
-- +1 cql_int32 int_lit_foo = 0;
-- +1 cql_string_ref a_string = NULL;
-- +1 cql_string_ref str_foo = NULL;
-- +1 cql_int32 int_cast_foo = 0;
-- +1 cql_int32 int_sql_val = 0;
-- +1 int_lit_foo = 1;
-- +1 cql_set_string_ref(&a_string, _literal_26_abc_my_proc_check_type);
-- +1 cql_set_string_ref(&str_foo, a_string);
-- +1 int_cast_foo = 1;
-- +1 _rc_ = cql_prepare(_db_, &_temp_stmt,
-- +1   "SELECT 1");
CREATE PROC my_proc_check_type()
BEGIN
  -- LET int_lit_foo := type_check(1 AS int NOT NULL);
  -- LET a_string := "abc";
  -- LET str_foo := type_check(a_string AS text NOT NULL);
  -- LET int_cast_foo := type_check(CAST(1 AS INTEGER<foo>) AS INTEGER<foo> NOT NULL);
  -- LET int_sql_val := (SELECT type_check(1 AS INTEGER NOT NULL));
END;
--------------------------------------------------------------------
-------------------- add new tests before this point ---------------
--------------------------------------------------------------------
LET this_is_the_end := 0xf00d;

CREATE PROC end_proc() BEGIN END;

-- TEST: end marker -- this is the last test
-- + cql_NULLable_int32 end_marker = { .is_NULL = 1 };
-- + cql_code cql_startup(sqlite3 *_NonNULL _db_)
DECLARE end_marker INTEGER;
--------------------------------------------------------------------
