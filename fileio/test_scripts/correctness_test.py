#!/usr/bin/env python3

import subprocess
import sys
import json

def log(msg):
    if '--grader' not in sys.argv:
        print(msg)


def shell_return(shell_cmd, suppress=False):
    if not suppress:
        log('-> ' + shell_cmd)
    return subprocess.run(
        shell_cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        shell=True
    ).returncode

def files_same(file1, file2):
    return shell_return(f'diff {file1} {file2}', suppress=True) == 0


def byte_cat(infile, outfile, outfile2):
    return shell_return(f'./byte_cat {infile} {outfile}') == 0 \
        and files_same(infile, outfile)

def reverse_byte_cat(infile, outfile, outfile2):
    return shell_return(f'./reverse_byte_cat {infile} {outfile}') == 0 \
        and shell_return(f'./reverse_byte_cat {outfile} {outfile2}') == 0 \
        and files_same(infile, outfile2)

def create_block_cat(block_size):
    def block_cat(infile, outfile, outfile2):
        return shell_return(f'./block_cat {block_size} {infile} {outfile}') == 0 \
            and files_same(infile, outfile)
    return block_cat

def create_reverse_block_cat(block_size):
    def reverse_block_cat(infile, outfile, outfile2):
        return shell_return(f'./reverse_block_cat {block_size} {infile} {outfile}') == 0 \
            and not files_same(infile, outfile) \
            and shell_return(f'./reverse_block_cat {block_size} {outfile} {outfile2}') == 0 \
            and files_same(infile, outfile2)
    return reverse_block_cat

def random_block_cat(infile, outfile, outfile2):
    return shell_return(f'./random_block_cat {infile} {outfile}') == 0 \
        and files_same(infile, outfile)

def rot13(infile, outfile, outfile2):
    assert shell_return(f'cp {infile} {outfile}') == 0
    return shell_return(f'./rot13 {outfile}') == 0 \
        and shell_return(f'./rot13 {outfile}') == 0 \
        and files_same(infile, outfile)



def runtests(tests):
    infile = '/tmp/infile'
    outfile = '/tmp/outfile'
    outfile2 = '/tmp/outfile2'
    integrity = '/tmp/integrity'
    assert shell_return(f'touch {infile}', suppress=True) == 0
    assert shell_return(f'dd if=/dev/urandom of={infile} bs=4096 count=20', suppress=True) == 0
    assert shell_return(f'touch {integrity}', suppress=True) == 0
    assert shell_return(f'cp {infile} {integrity}', suppress=True) == 0

    for test in tests:
        # Truncate output files
        assert shell_return(f'> {outfile}', suppress=True) == 0
        assert shell_return(f'> {outfile2}', suppress=True) == 0

        log(f'\033[32mrunning test: {test}\033[0m')

        testfun = tests[test]
        passed = testfun(infile, outfile, outfile2)

        if not files_same(infile, integrity):
            print('oops, your program modified the input file')
            tests[test] = False
        else:
            tests[test] = passed

    log('Your results will follow. Each named test will have a true/false attached')
    log('to it, indicating whether or not you passed.')
    log('If a test failed, you should debug with a small sample file to see how your output is different.')
    log('Also, if there is sanitizer output, that would be a good place to start.')
    print(json.dumps(tests, indent=4))

    assert shell_return(f'rm -- {infile} {outfile} {outfile2} {integrity}', suppress=True) == 0


runtests({
    'byte_cat': byte_cat,
    'reverse_byte_cat': reverse_byte_cat,
    'block_cat_1': create_block_cat(1),
    'block_cat_17': create_block_cat(17),
    'block_cat_334': create_block_cat(334),
    'block_cat_huge': create_block_cat(8192),
    'reverse_block_cat_1': create_reverse_block_cat(1),
    'reverse_block_cat_13': create_reverse_block_cat(13),
    'reverse_block_cat_987': create_reverse_block_cat(411),
    'reverse_block_cat_huge': create_reverse_block_cat(8192),
    'random_block_cat': random_block_cat,
    'rot13': rot13
})