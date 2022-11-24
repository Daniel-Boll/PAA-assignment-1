import streamlit as st
import pandas as pd
import numpy as np
import json
import altair as alt

st.title(
    "Benchmark of the C++ implementation of both Counting and Quick sort algorithm"
)


@st.cache
def load_data():
    with open("benchmark.json", "r") as f:
        data = json.load(f)
    return data


benchmark_data = load_data()

st.caption(f"Builded as {benchmark_data['context']['library_build_type']}")

st.header("Full benchmark data")

with st.expander("See details"):
    st.json(benchmark_data)

st.header("CPU information")

st.markdown(f"`Hostname`: {benchmark_data['context']['host_name']}")
st.markdown(f"`N° cpu(s)`: {benchmark_data['context']['num_cpus']}")
st.markdown(f"`MHz/cpu`: {benchmark_data['context']['mhz_per_cpu']}")
st.markdown(
    f"`CPU Scaling enabled`: {benchmark_data['context']['cpu_scaling_enabled']}"
)

st.subheader("Caches info")

caches = benchmark_data["context"]["caches"]
caches_column = st.columns(len(caches))

# Get cache and index
for index, cache in enumerate(caches_column):
    cache.metric(
        f"Level {caches[index]['level']}",
        f"{caches[index]['type']}",
        f"{caches[index]['size']} bits",
    )


def separate_benchmark_data(benchmarks):
    """
    Separate the benchmark data into a dictonary.
    """

    separated_benchmarks = {}
    for benchmark in benchmarks:
        # If there's a key "aggregate_name" in the benchmark skip it
        if "aggregate_name" in benchmark:
            continue

        classes, size = benchmark["run_name"].rsplit("/", 1)
        name, operation = classes.split("/")

        if name not in separated_benchmarks:
            separated_benchmarks[name] = {}

        if size not in separated_benchmarks[name]:
            separated_benchmarks[name][size] = {}

        separated_benchmarks[name][size][operation] = benchmark["real_time"]

    return separated_benchmarks


separated_benchmarks = separate_benchmark_data(benchmark_data["benchmarks"])


def get_all_values_of_benchmark(name: str, operation: str) -> list:
    """
    Get all the values of a benchmark for a specific operation.

    The values returned are in the format [size, value].
    """
    values = []
    for size, operations in separated_benchmarks[name].items():
        values.append([int(size), operations[operation]])

    return values


for benchmark_name, benchmark_data in separated_benchmarks.items():
    st.header(f"{benchmark_name}")

    asc_tab, desc_tab, rand_tab, part_tab, cmp_tab = st.tabs(
        ["Ascendent", "Decendent", "Random", "Partially sorted", "Comparison"]
    )

    def tab(tab, operation):
        values = get_all_values_of_benchmark(benchmark_name, operation)
        # Compare the values to the function $n(log2(n))$ if the benchmark is QuickSort
        # and n+k where k is 5000 if the benchmark is CountingSort
        func_values = []
        if benchmark_name == "BM_QuickSort":
            func_values = [
                [value[0], (value[0] * np.log2(value[0]))] for value in values
            ]
        elif benchmark_name == "BM_CountingSort":
            func_values = [[value[0], (value[0] + 5000)] for value in values]

        df = pd.DataFrame(values + func_values, columns=["Size", "Time (ns)"])
        df["Type"] = ["Benchmark"] * len(values) + [
            "nlog(n)" if benchmark_name == "BM_QuickSort" else "n+k"
        ] * len(func_values)
        # Altair chart
        chart = (
            alt.Chart(df)
            .mark_line()
            .encode(
                x=alt.X("Size", title="Size of the array"),
                y=alt.Y("Time (ns)", title="Time (ns)"),
                color="Type",
            )
        ).interactive()
        tab.altair_chart(chart, use_container_width=True)

    with asc_tab:
        tab(asc_tab, "asc")
    with desc_tab:
        tab(desc_tab, "dec")
    with rand_tab:
        tab(rand_tab, "rand")
    with part_tab:
        tab(part_tab, "part_sorted")

    with cmp_tab:
        asc_values = get_all_values_of_benchmark(benchmark_name, "asc")
        desc_values = get_all_values_of_benchmark(benchmark_name, "dec")
        rand_values = get_all_values_of_benchmark(benchmark_name, "rand")
        part_values = get_all_values_of_benchmark(benchmark_name, "part_sorted")

        df = pd.DataFrame(
            asc_values + desc_values + rand_values + part_values,
            columns=["Size", "Time (ns)"],
        )
        df["Type"] = (
            ["Ascendent"] * len(asc_values)
            + ["Decendent"] * len(desc_values)
            + ["Random"] * len(rand_values)
            + ["Partially sorted"] * len(part_values)
        )

        chart = (
            alt.Chart(df)
            .mark_line()
            .encode(
                x=alt.X("Size", title="Size of the array"),
                y=alt.Y("Time (ns)", title="Time (ns)"),
                color="Type",
            )
        ).interactive()
        cmp_tab.altair_chart(chart, use_container_width=True)


st.header(f"Comparison")

asc_tab, desc_tab, rand_tab, part_tab = st.tabs(
    ["Ascendent", "Decendent", "Random", "Partially sorted"]
)


def get_comparison_values_of_benchmark(operation: str) -> list:
    couting_asc_values = get_all_values_of_benchmark("BM_CountingSort", operation)
    quick_asc_values = get_all_values_of_benchmark("BM_QuickSort", operation)

    return [couting_asc_values, quick_asc_values]


def tab_content(tab, operation: str):
    couting_values, quick_values = get_comparison_values_of_benchmark(operation)
    df = pd.DataFrame(couting_values + quick_values, columns=["Size", "Time (ns)"])

    df["Type"] = ["Counting"] * len(couting_values) + ["Quick"] * len(quick_values)

    chart = (
        alt.Chart(df)
        .mark_line()
        .encode(
            x=alt.X("Size", title="Size of the array"),
            y=alt.Y("Time (ns)", title="Time (ns)"),
            color="Type",
        )
    ).interactive()
    tab.altair_chart(chart, use_container_width=True)


with asc_tab:
    tab_content(asc_tab, "asc")
with desc_tab:
    tab_content(desc_tab, "dec")
with rand_tab:
    tab_content(rand_tab, "rand")
with part_tab:
    tab_content(part_tab, "part_sorted")
