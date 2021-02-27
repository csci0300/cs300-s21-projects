#!/usr/bin/env python3

import subprocess
import sys
import json
import tempfile

def log(msg):
    if '--grader' not in sys.argv:
        print(msg)


def silent_shell(cmd):
    sp = subprocess.run(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        shell=True
    )
    if sp.returncode != 0:
        print(f'fatal: the command `${cmd}` failed')
        print(str(sp.stdout, 'utf-8'))
        print(str(sp.stderr, 'utf-8'))
        sys.exit(1)

def get_time_field(output, fieldname) -> int:
    try:
        fieldstart = output.index(fieldname) + len(fieldname)
        fieldend = output.index('\\n', fieldstart)
        # parse the number in the field (might be a percentage)
        return float(output[fieldstart:fieldend].replace('%', ''))
    except:
        return None


def time_program(progcmd):
    sp = subprocess.run(
        ['/usr/bin/time', '--verbose', '--'] + progcmd.split(' '),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    if sp.returncode != 0:
        return str(sp.stdout, 'utf-8') + str(sp.stderr, 'utf-8')

    time_output = str(sp.stderr)

    return {
        'cpu': get_time_field(time_output, 'Percent of CPU this job got:'),
        'stime': get_time_field(time_output, 'System time (seconds):'),
        'utime': get_time_field(time_output, 'User time (seconds):'),
        'mrss': get_time_field(time_output, 'Maximum resident set size (kbytes):'),
        'arss': get_time_field(time_output, 'Average resident set size (kbytes):'),
    }






def byte_cat(infile, outfile):
    return f'./byte_cat {infile} {outfile}'

def reverse_byte_cat(infile, outfile):
    return f'./reverse_byte_cat {infile} {outfile}'

def block_cat(infile, outfile):
    return f'./block_cat 32 {infile} {outfile}'

def reverse_block_cat(infile, outfile):
    return f'./block_cat 32 {infile} {outfile}'

def random_block_cat(infile, outfile):
    return f'./random_block_cat {infile} {outfile}'

def stride_cat(infile, outfile):
    return f'./stride_cat 1 1024 {infile} {outfile}'


def runtests(tests):
    results = {}
    for testname in tests:
        results[testname] = {}

    infile = '/tmp/infile'
    outfile = '/tmp/outfile'
    silent_shell(f'dd if=/dev/urandom of={infile} bs=1M count=10')

    def suite(makecmd, impl):
        log(f'\033[31mrunning test suite: {impl}\033[0m')
        silent_shell('make clean')
        silent_shell(makecmd)

        for testname in tests:
            log(f'\033[32mrunning test: {impl}::{testname}\033[0m')
            silent_shell(f'> {outfile}')
            prog_str = tests[testname](infile, outfile)
            log('-> ' + prog_str)
            results[testname][impl] = time_program(prog_str)


    suite('make -B IMPL=stdio', 'stdio')
    suite('CFLAGS=-DBUFFER_SIZE=4096 make -B IMPL=student', 'student')

    metrics = {}
    for testname in tests:
        if type(results[testname]['stdio']) is not dict:
            print('ERROR: stdio program failed. This should not happen, please contact the course staff')
            print(results[testname]['stdio'])
            sys.exit(1)
        if results[testname]['student'] is None:
            log('student program failed')
            metrics[testname] = 'student test failed'
            continue

        stdio_time = results[testname]['stdio']['stime'] + results[testname]['stdio']['utime']
        student_time = results[testname]['student']['stime'] + results[testname]['student']['utime']

        if stdio_time == 0.0:
            stdio_time = 0.001

        ratio = student_time / stdio_time
        metrics[testname] = ratio
        log('performance result: %s: stdio=%.2fs, student=%.2fs, ratio=%.2f' \
                % (testname, stdio_time, student_time, student_time / stdio_time))

    print(json.dumps(metrics, indent=4))

    silent_shell('make clean')
    silent_shell(f'rm -- {infile} {outfile}')



runtests({
    'byte_cat': byte_cat,
    'reverse_byte_cat': reverse_byte_cat,
    'block_cat': block_cat,
    'reverse_block_cat': reverse_block_cat,
    'random_block_cat': random_block_cat,
    'stride_cat': stride_cat
})




