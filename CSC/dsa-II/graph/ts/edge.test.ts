import { expect, test } from 'vitest';
import Edge from './edge';

test('compares a < b, a > b, a = b', () => {
    let a = new Edge<number>(1, 1, 0);
    let b = new Edge<number>(1,1,1);
    
    expect(a.compare(b)).toBe(-1);

    a = new Edge<number>(1, 1, 1);
    b = new Edge<number>(1,1,0);

    expect(a.compare(b)).toBe(1);

    a = new Edge<number>(1, 1, 0);
    b = new Edge<number>(1,1,0);
    expect(a.compare(b)).toBe(0);
})

