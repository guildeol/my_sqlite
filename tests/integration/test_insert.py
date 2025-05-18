def test_should_insert_into_and_read_from_table(shell):
    script = 'CREATE TABLE test_table (id, name)                        \n' \
             'INSERT INTO test_table (id, name) VALUES (1, "Alice")     \n' \
             'SELECT * FROM test_table \n'

    response = shell.run(script)

    assert '1|"Alice"' in response

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
