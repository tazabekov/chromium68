# Tab Ranker

The Tab Ranker is an experimental model used to predict tab reactivation. We
are setting up experiments to use this model as a factor in deciding which tabs
to discard when the system is low on memory.

## Model

The input to the model is a list of metrics about a background tab, such as
whether it is pinned, how long it has been in the background, and how many
events it has received. The output is a floating point value, where a higher
number suggests the tab is more likely to be reactivated than a lower number.
The range of possible outputs is dependent on the model; what matters is the
relative ordering of scores, not the precise value of a score.

The metrics used for inference are the same metrics logged to UKM by
TabMetricsLogger. Using these metrics, we trained a
[DNN](https://en.wikipedia.org/wiki/Deep_learning#Deep_neural_networks) model.
The inferencing code in `native_inference.cc` consists of the model weights
generated by TensorFlow and basic operations to execute the model over an
example.

See [native_inference.md] for details on how the tf.native code is cleaned up
for Chromium.

## Example preprocessing

Examples are provided to the model as an array of floats, with each element
corresponding to a feature. For scalar metrics like WasRecentlyActive(), this
feature is simply a 0 or 1.

A metric with multiple possible values, like an enum or bucketized number, is
broken into a number of features, each corresponding to one of its possible
values. For example, a value of 7 for the PageTransitionCoreType metric is
represented as a 1 in the `PageTransitionCoreType_7` feature. See
https://en.wikipedia.org/wiki/One-hot.

The AssistRanker handles this example preprocessing based on the configuration
provided in example_preprocessor_config.pb. This can be examined with the
`print_example_preprocessor_config.py` utility:

```
  ./components/assist_ranker/print_example_preprocessor_config.py \
  out/Release \
  chrome/browser/resource_coordinator/tab_ranker/example_preprocessor_config.pb
```

## Scoring a tab

Use `TabActivityWatcher::CalculateReactivationScore()` to get a score from a
WebContents.

Internally, `TabScorePredictor::ScoreTab` is the interface to the model. The
caller needs to track a number of metrics about the tab and its window. These
metrics must be calculated in the same way they were logged, as otherwise the
results will not be valid.