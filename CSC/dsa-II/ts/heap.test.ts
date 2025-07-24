import { expect, test } from 'vitest';
import Heap, { HeapPriority, type HeapComparator, type HeapItems } from './heap';

test('Min heap pops correctly', () => {
    const items: HeapItems<number> = [5, 4, 1, 3, 2, 69];
    const comparator: HeapComparator<number> = (a: number, b: number) => {
        if(a < b) return HeapPriority.First;
        return HeapPriority.Second;
    };
    const H = new Heap(items, comparator);
    const expected = [1,2,3,4,5,69];
    for(let i = 0; i < items.length; i++){
        expect(H.peekPriorityItem()).toBe(expected[i]);
        expect(H.popPriorityItem()).toBe(expected[i]);
    }
});
 
