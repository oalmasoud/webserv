#!/bin/bash

# ============================================================
# HTTP Request Tester
# Tests HttpRequest parsing with raw HTTP request files
# ============================================================

TESTER="./request_tester"
TEST_DIR="request_tests"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

PASS_COUNT=0
FAIL_COUNT=0
TOTAL_COUNT=0

print_header() {
    echo ""
    echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
    echo -e "${BLUE}  $1${NC}"
    echo -e "${BLUE}═══════════════════════════════════════════════════════════${NC}"
}

print_subheader() {
    echo ""
    echo -e "${YELLOW}──────────────────────────────────────────────────────────${NC}"
    echo -e "${YELLOW}  $1${NC}"
    echo -e "${YELLOW}──────────────────────────────────────────────────────────${NC}"
}

# Test function
# Args: test_name request_content expected_parseResult [expected_method] [expected_uri] [expected_host] [expected_port]
run_test() {
    local test_name="$1"
    local request_content="$2"
    local expected_parseResult="$3"
    local expected_method="$4"
    local expected_uri="$5"
    local expected_host="$6"
    local expected_port="$7"
    
    TOTAL_COUNT=$((TOTAL_COUNT + 1))
    
    # Create test file with proper CRLF line endings using printf
    local test_file="$TEST_DIR/test_${TOTAL_COUNT}.txt"
    printf "%b" "$request_content" > "$test_file"
    
    # Run tester
    output=$($TESTER "$test_file" 2>&1)
    exit_code=$?
    
    # Parse output
    actual_parseResult=$(echo "$output" | grep "^parseResult=" | cut -d'=' -f2)
    
    # Compare results
    local passed=true
    local errors=""
    
    if [ "$actual_parseResult" != "$expected_parseResult" ]; then
        passed=false
        errors="${errors}   Expected parseResult=$expected_parseResult, got $actual_parseResult\n"
    fi
    
    # If parse succeeded, check fields
    if [ "$expected_parseResult" = "true" ] && [ "$actual_parseResult" = "true" ]; then
        if [ -n "$expected_method" ]; then
            actual_method=$(echo "$output" | grep "^method=" | cut -d'=' -f2)
            if [ "$actual_method" != "$expected_method" ]; then
                passed=false
                errors="${errors}   Expected method=$expected_method, got $actual_method\n"
            fi
        fi
        
        if [ -n "$expected_uri" ]; then
            actual_uri=$(echo "$output" | grep "^uri=" | cut -d'=' -f2)
            if [ "$actual_uri" != "$expected_uri" ]; then
                passed=false
                errors="${errors}   Expected uri=$expected_uri, got $actual_uri\n"
            fi
        fi
        
        if [ -n "$expected_host" ]; then
            actual_host=$(echo "$output" | grep "^host=" | cut -d'=' -f2)
            if [ "$actual_host" != "$expected_host" ]; then
                passed=false
                errors="${errors}   Expected host=$expected_host, got $actual_host\n"
            fi
        fi
        
        if [ -n "$expected_port" ]; then
            actual_port=$(echo "$output" | grep "^port=" | cut -d'=' -f2)
            if [ "$actual_port" != "$expected_port" ]; then
                passed=false
                errors="${errors}   Expected port=$expected_port, got $actual_port\n"
            fi
        fi
    fi
    
    # Check exit code matches parse result
    if [ "$expected_parseResult" = "true" ] && [ $exit_code -ne 0 ]; then
        passed=false
        errors="${errors}   Expected exit code 0, got $exit_code\n"
    elif [ "$expected_parseResult" = "false" ] && [ $exit_code -eq 0 ]; then
        passed=false
        errors="${errors}   Expected non-zero exit code, got 0\n"
    fi
    
    if [ "$passed" = true ]; then
        echo -e "${GREEN}✅ PASS${NC} [$TOTAL_COUNT] $test_name"
        PASS_COUNT=$((PASS_COUNT + 1))
        return 0
    else
        echo -e "${RED}❌ FAIL${NC} [$TOTAL_COUNT] $test_name"
        echo -e "${RED}${errors}${NC}"
        FAIL_COUNT=$((FAIL_COUNT + 1))
        return 1
    fi
}

# ============================================================
# Check if tester binary exists
# ============================================================

print_header "HTTP Request Tester"
ls -l "$TESTER"
if [ ! -f "$TESTER" ]; then
    echo -e "${RED}❌ Error: $TESTER not found${NC}"
    ls 
    echo -e "${YELLOW}Please compile first: make request_tester${NC}"
    exit 1
fi

# Create test directory
mkdir -p "$TEST_DIR"

# ============================================================
# VALID REQUEST TESTS
# ============================================================

print_subheader "Valid Request Tests"

# Test 1: Simple GET request
run_test "Simple GET request" \
$'GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n' \
"true" "GET" "/" "localhost" "8080"

# Test 2: GET with path
run_test "GET with path" \
$'GET /index.html HTTP/1.1\r\nHost: example.com:80\r\n\r\n' \
"true" "GET" "/index.html" "example.com" "80"

# Test 3: POST with body
run_test "POST with body" \
$'POST /api/users HTTP/1.1\r\nHost: localhost:8080\r\nContent-Type: application/json\r\nContent-Length: 24\r\n\r\n{"name":"John","age":30}' \
"true" "POST" "/api/users" "localhost" "8080"

# Test 4: DELETE request
run_test "DELETE request" \
$'DELETE /api/users/123 HTTP/1.1\r\nHost: localhost:8080\r\n\r\n' \
"true" "DELETE" "/api/users/123" "localhost" "8080"

# Test 5: GET with query string
run_test "GET with query string" \
$'GET /search?q=test&page=1 HTTP/1.1\r\nHost: localhost:8080\r\n\r\n' \
"true" "GET" "/search" "localhost" "8080"

# Test 6: Multiple headers
run_test "Multiple headers" \
$'GET /page HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: Mozilla/5.0\r\nAccept: text/html\r\nAccept-Language: en-US\r\nConnection: keep-alive\r\n\r\n' \
"true" "GET" "/page" "localhost" "8080"

# Test 7: POST with form data
run_test "POST with form data" \
$'POST /submit HTTP/1.1\r\nHost: localhost:8080\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 23\r\n\r\nusername=test&pass=1234' \
"true" "POST" "/submit" "localhost" "8080"

# ============================================================
# INVALID REQUEST TESTS
# ============================================================

print_subheader "Invalid Request Tests"

# Test 8: Missing Host header
run_test "Missing Host header" \
$'GET / HTTP/1.1\r\n\r\n' \
"false"

# Test 9: Malformed request line
run_test "Malformed request line" \
$'GET HTTP/1.1\r\nHost: localhost:8080\r\n\r\n' \
"false"

# Test 10: Invalid method
run_test "Invalid method" \
$'INVALID / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n' \
"false"

# Test 11: Missing HTTP version
run_test "Missing HTTP version" \
$'GET /\r\nHost: localhost:8080\r\n\r\n' \
"false"

# Test 12: Body size mismatch
run_test "Body size mismatch" \
$'POST /api HTTP/1.1\r\nHost: localhost:8080\r\nContent-Length: 100\r\n\r\nshort body' \
"false"

# Test 13: No CRLF separator
run_test "No CRLF separator" \
$'GET / HTTP/1.1\nHost: localhost:8080\n\n' \
"false"

# Test 14: Empty request
run_test "Empty request" \
$'' \
"false"

# ============================================================
# EDGE CASES
# ============================================================

print_subheader "Edge Case Tests"

# Test 15: Root path
run_test "Root path" \
$'GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n' \
"true" "GET" "/" "localhost" "8080"

# Test 16: Long URI
run_test "Long URI" \
$'GET /very/long/path/to/resource/file.html HTTP/1.1\r\nHost: localhost:8080\r\n\r\n' \
"true" "GET" "/very/long/path/to/resource/file.html" "localhost" "8080"

# Test 17: Different port
run_test "Different port" \
$'GET / HTTP/1.1\r\nHost: example.com:3000\r\n\r\n' \
"true" "GET" "/" "example.com" "3000"

# Test 18: Zero content length
run_test "Zero content length" \
$'POST / HTTP/1.1\r\nHost: localhost:8080\r\nContent-Length: 0\r\n\r\n' \
"true" "POST" "/" "localhost" "8080"

# ============================================================
# SUMMARY
# ============================================================

print_header "Test Summary"
echo "Total Tests: $TOTAL_COUNT"
echo -e "${GREEN}Passed: $PASS_COUNT${NC}"
echo -e "${RED}Failed: $FAIL_COUNT${NC}"


if [ $FAIL_COUNT -eq 0 ]; then
    echo ""
    echo -e "${GREEN}🎉 All tests passed!${NC}"
    exit 0
else
    echo ""
    echo -e "${RED}❌ Some tests failed${NC}"
    exit 1
fi
