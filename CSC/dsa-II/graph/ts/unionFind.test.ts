import { expect, test } from 'vitest';
import UnionFind, { UnionFindNode } from './unionFind';

test('initializes nodes correctly', () => {

    let uf = new UnionFind<number>();
    for(let i = 0; i < 10; i++) {
        uf.add(i)
        expect(uf.nodes[i].parent).toBe(uf.nodes[i]);
        expect(uf.find(uf.nodes[i])).toBe(uf.nodes[i]);
    }
    
})

test('unions nodes correctly', () => {

    let uf = new UnionFind<number>();
    for(let i = 0; i < 10; i++) {
        uf.add(i)
    }

    for(let i = 1; i < uf.nodes.length; i++) {
        uf.union(uf.nodes[i - 1], uf.nodes[i]);
        
    }
    expect(uf.nodes[0].size).toEqual(10);
    
})

