def test_should_create_table(shell):
    script = 'CREATE TABLE test_table (id, name)    \n' \
             '.tables                               \n'

    response = shell.run(script)

    assert 'test_table' in response, 'Table "test_table" was not created successfully'

def test_should_create_multiple_tables(shell):
    script = 'CREATE TABLE test_table1 (id, name)                     \n' \
             'CREATE TABLE test_table2 (id, name)                     \n' \
             '.tables                                                 \n'

    response = shell.run(script)

    assert 'test_table1' in response, 'Table "test_table1" was not created successfully'
    assert 'test_table2' in response, 'Table "test_table2" was not created successfully'
