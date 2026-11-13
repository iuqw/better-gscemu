# Skill template

Copy this file to `.opencode/skills/<id>/SKILL.md` and replace the placeholders.

````markdown
---
name: <Display name>
description: <What it does and when to use it; include better-gscemu and the trigger>
---

# <Display name>

<One sentence describing the task.>

## Read first

- `AGENTS.md`
- `<nearest module AGENTS.md>`
- `<relevant source or test files>`

## Run

```bash
<exact command>
```

## Workflow

1. <Inspect or reproduce>
2. <Change the code>
3. <Update tests, corpus, dictionary, or docs>
4. <Verify>

## Rules

- <Repo-specific invariant>
- <Boundary or thing not to do>

See [reference](references/<file>.md) for details.
````
