
import os, psutil

from memory_leak import memory_leak


def get_mem_usage_mb():
    return psutil.Process(os.getpid()).memory_info().rss / 1024 ** 2


def test_memory_leak():

    P = 204
    r = 34
    N = 64

    mem = []
    
    for i in range(5):
            
        memory_leak(P*(r+i)*N*N)
        mem.append(get_mem_usage_mb())
        print(f'--> Memory usage: {mem[-1]} MiB')

    # The list "mem" takes a few bytes but less than 100 MiB
    assert( mem[-1] < mem[0] + 100. )


if __name__ == '__main__':
    test_memory_leak()
