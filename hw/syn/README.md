# OpenTitan Synthesis Flow

This readme gives some quick instructions on how to run synthesis in OpenTitan, using top_earlgrey as an example.

## Normal Synthesis Through dvsim

To run synthesis through dvsim, use a command like the following:

```
$ cd $REPO_TOP
$ ./util/dvsim/dvsim.py -t dc hw/top_earlgrey/syn/chip_earlgrey_asic_syn_cfg.hjson --purge --local
```

This comment will invoke dvsim to run synthesis and terminate upon success or failure.
The synthesis results are placed in the scratch area under `$SCRATCH_ROOT/{branch_name}/chip_earlgrey_asic-syn-dc/` for this example command.

The main script that powers synthesis is `hw/syn/tools/dc/run-syn.tcl`.

## Re-run Interactive Synthesis

Assuming the above synthesis steps produces an error or creates a situation where we must run synthesis interactively for debug, it can be done as follows.

When the normal synthesis steps are run (see example above), an output file called `env_variables.tcl` is created in the synthesis scratch area, `$SCRATCH_ROOT/{branch_name}/chip_earlgrey_asic-syn-dc/default` in the above example.

To re-run this synthesis interactively, do the following:

```
$ cd $SCRATCH_ROOT/{branch_name}/chip_earlgrey_asic-syn-dc/default/syn-icarus
$ dc_shell

```

The above command opens dc_shell in the scratch area.
Once dc_shell is open, do the following:

```
$ source ../env_variables.tcl
$ source $REPO_TOP/hw/syn/tools/dc/run-syn.tcl

```

Now, synthesis will begin again but will not exit dc_shell upon completion.
If you do not wish for synthesis to run that far, `run_syn.tcl` can be directly modified to add or skip steps.
