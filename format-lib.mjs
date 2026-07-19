// Format mudlib LPC with the driver's formatter (width 80, indent 4).
//
// Same safety gates as driver/tools/lpc-syntax/bin/format-corpus.mjs:
// token-sequence equivalence, literal byte-identity, idempotency. A
// file failing any gate is reported, left untouched, and the run
// exits nonzero.
//
// Usage: node format-lib.mjs [--check] file.c [file2.c ...]
//        find lib -name '*.c' | node format-lib.mjs [--check]
import { readFileSync, writeFileSync } from 'node:fs';
import { fileURLToPath } from 'node:url';
import { dirname, join } from 'node:path';

const here = dirname(fileURLToPath(import.meta.url));
const syntax = join(here, 'driver', 'tools', 'lpc-syntax');
const { formatLPC } = await import(join(syntax, 'format.mjs'));
const { tokenize } = await import(join(syntax, 'tokenizer.mjs'));

const OPTS = { printWidth: 80, indentSize: 4 };

const args = process.argv.slice(2);
const checkOnly = args.includes('--check');
const fileArgs = args.filter((a) => a !== '--check');
const files = fileArgs.length ? fileArgs : readFileSync(0, 'utf8').split('\n').filter(Boolean);

function sig (src) {
    return tokenize(src).filter((t) => t.kind !== 'whitespace')
        .map((t) => t.kind + ':' + (t.kind === 'directive' ? t.text.replace(/[ \t]+$/g, '') : t.text))
        .join('\n');
}

const LITERAL_KINDS = new Set(['string', 'template', 'textblock', 'char', 'comment', 'directive']);
function literalText (src) {
    return tokenize(src).filter((t) => LITERAL_KINDS.has(t.kind))
        .map((t) => (t.kind === 'directive' ? t.text.replace(/[ \t]+$/g, '') : t.text))
        .join(String.fromCharCode(0));
}

let written = 0, unchanged = 0, wouldChange = 0, errors = 0;
for (const f of files) {
    let src;
    try {
        src = readFileSync(f, 'utf8');
    } catch (e) {
        errors++;
        console.error(`READ ERROR ${f}: ${e.message}`);
        continue;
    }
    let out;
    try {
        out = formatLPC(src, OPTS);
    } catch (e) {
        errors++;
        console.error(`FORMAT ERROR ${f}: ${e.message}`);
        continue;
    }
    if (out === src) { unchanged++; continue; }
    if (sig(src) !== sig(out)) {
        errors++;
        console.error(`TOKEN MISMATCH, refusing to write: ${f}`);
        continue;
    }
    if (literalText(src) !== literalText(out)) {
        errors++;
        console.error(`LITERAL CONTENT CHANGED, refusing to write: ${f}`);
        continue;
    }
    if (formatLPC(out, OPTS) !== out) {
        errors++;
        console.error(`NOT IDEMPOTENT, refusing to write: ${f}`);
        continue;
    }
    if (checkOnly) {
        wouldChange++;
        console.log(`would reformat: ${f}`);
    } else {
        writeFileSync(f, out);
        written++;
        console.log(`reformatted: ${f}`);
    }
}

console.log(JSON.stringify({ total: files.length, written, wouldChange, unchanged, errors }));
process.exit(errors > 0 || (checkOnly && wouldChange > 0) ? 1 : 0);
