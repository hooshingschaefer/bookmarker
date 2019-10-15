
CREATE OR REPLACE FUNCTION add_user(username varchar(30), password integer) returns int as 
$$
DECLARE 
    row_count integer;
BEGIN    
    BEGIN
        INSERT INTO test.users values(username, password);
        return 2;
    EXCEPTION WHEN unique_violation THEN 
        return 0;
    end;

END;
$$
LANGUAGE 'plpgsql';

