#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Check exit command terminates shell" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 249 ]
}

@test "Check cd without arguments" {
    initial_dir=$(pwd)
    run ./dsh <<EOF
cd
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"$initial_dir"* ]]
}

@test "Check cd to /tmp should succeed" {
    run ./dsh <<EOF
cd /tmp
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"/tmp"* ]]
}

@test "Check cd to invalid directory should fail" {
    run ./dsh <<EOF
cd /invalid_dir
pwd
EOF
    [ "$status" -eq 0 ]
}

@test "Check echo prints correctly" {
    run ./dsh <<EOF
echo "Hello world!"
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello world!"* ]]
}

@test "Check echo prints correctly with extra space" {
    run ./dsh <<EOF
echo "Hello    world!"
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello    world!"* ]]
}

@test "Check dragon command" {
    run ./dsh <<EOF
dragon
EOF
    [ "$status" -eq 0 ]
}

@test "Check leading whitespace is handled correctly" {
    run ./dsh <<EOF
     ls -l
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"$(ls -l)"* ]]
}

@test "Check extra whitespace is handled correctly" {
    run ./dsh <<EOF
ls     -l
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"$(ls -l)"* ]]
}

@test "Check empty input warning" {
    run ./dsh <<EOF

EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"$CMD_WARN_NO_CMD"* ]]
}