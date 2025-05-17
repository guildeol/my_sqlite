def test_should_create_table(shell):
    script = 'CREATE TABLE test_table (id, name)    \n' \
             '.tables                               \n'

    response = shell.run(script)

    assert 'test_table' in response, 'Table "test_table" was not created successfully'


def test_should_insert_into_and_read_from_table(shell):
    script = 'CREATE TABLE test_table (id, name)                        \n' \
             'INSERT INTO test_table (id, name) VALUES (1, "Alice")     \n' \
             'SELECT * FROM test_table \n'

    response = shell.run(script)

    assert '1|"Alice"' in response

def test_should_create_multiple_tables(shell):
    script = 'CREATE TABLE test_table1 (id, name)                     \n' \
             'CREATE TABLE test_table2 (id, name)                     \n' \
             '.tables                                                  \n'

    response = shell.run(script)

    assert 'test_table1' in response, 'Table "test_table1" was not created successfully'
    assert 'test_table2' in response, 'Table "test_table2" was not created successfully'

def test_should_insert_into_right_table(shell):
    script = 'CREATE TABLE test_table1 (id, name)                     \n' \
             'CREATE TABLE test_table2 (id, name)                     \n' \
             'INSERT INTO test_table1 (id, name) VALUES (1, "Alice")  \n' \
             'SELECT * FROM test_table1 \n'

    response = shell.run(script)

    assert '1|"Alice"' in response

def test_should_insert_multiple_rows(shell):
    script = 'CREATE TABLE test_table (id, name)                     \n' \
             'INSERT INTO test_table (id, name) VALUES (1, "Alice")  \n' \
             'INSERT INTO test_table (id, name) VALUES (2, "Bob")    \n' \
             'SELECT * FROM test_table \n'

    response = shell.run(script)

    assert '1|"Alice"' in response
    assert '2|"Bob"' in response

