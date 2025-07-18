import { expect, test } from 'vitest';
import UnionFind, { UnionFindNode } from './unionFind';

test('initializes nodes correctly', () => {

    let uf = new UnionFind<number>();
    for(let i = 0; i < 10; i++) {
        uf.add(i)
        expect(uf.find(i).parent).toBe(uf.find(i));
    }
    
})

test('unions nodes correctly', () => {

    let uf = new UnionFind<number>();
    for(let i = 0; i < 10; i++) {
        uf.add(i)
    }

    for(let i = 1; i < uf.nodes.size; i++) {
        uf.union(i - 1, i);
        
    }
    expect(uf.find(0).size).toEqual(10);
    
})

