program
stmt_sequence
stmt_sequence; statement
stmt_sequence; statement; if-stmt
statement; read-stmt; if exp then stmt-sequence end
assign-stmt; read identifier; if simple-exp comparison-op simple-exp then statement end
identifier := exp; read n; if term < term then repeat-stmt end
result := simple-exp; read n; if factor < factor then repeat stmt-sequence until exp end
result := term; read n; if number < identifier then repeat stmt-sequence; statement until simple-exp comparison-op simple-exp end
result := factor; read n; if 0 < n then repeat statement; assign-stmt until term < term end
result := number; read n; if 0 < n then repeat assign-stmt; identifier := exp until factor < factor end
result := 1; read n; if 0 < n then repeat identifier := exp; n := simple-exp until identifier < number end
result := 1; read n; if 0 < n then repeat result := simple-exp; n := simple-exp addop term until n < 1 end
result := 1; read n; if 0 < n then repeat result := term; n := term - factor until n < 1 end
result := 1; read n; if 0 < n then repeat result := term mulop factor; n := identifier - number until n < 1 end
result := 1; read n; if 0 < n then repeat result := factor * identifier; n := n - 1 until n < 1 end
result := 1; read n; if 0 < n then repeat result := identifier * n; n := n - 1 until n < 1 end
result := 1; read n; if 0 < n then repeat result := result * n; n := n-1 until n < 1 end

one: CONST 1
result: CONST 1
n: SPACE
INPUT n
if:
LOAD n
JMPN end_if
	for:
		LOAD n
		JMPP end_for
		JMPZ end_for
		LOAD result
		MUL n
		STORE result
		LOAD n
		SUB ONE
		STORE n
		JMP for
	end_for:
	OUTPUT result
end_if:
read: SPACE

