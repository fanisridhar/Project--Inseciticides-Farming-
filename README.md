# Project: Insecticides & Farming Research Analysis

A comprehensive research data analysis project focused on scientific papers related to insecticides, farming, insect genetics, and agricultural research. This project includes data analysis, visualization, and network programming components.

## 📋 Project Overview

This project analyzes scientific research papers covering topics such as:
- Insect species and their genetics
- Insecticides and farming applications
- Research publications by authors, journals, and years
- Genetic studies and microbiology research
- Life history traits and ecological studies

## 📁 Project Structure

```
ProjectK/
├── backend/                    # Backend services (to be implemented)
├── frontend/                   # Frontend application (to be implemented)
├── FADS_Results/              # Analysis results and visualizations
│   ├── *.csv                  # Processed data files
│   ├── *.png                  # Graph visualizations
│   └── http_client.c          # HTTP client implementation
├── data-*.csv                 # Source data files
│   ├── data-countbyauthor.csv
│   ├── data-countbygenetics.csv
│   ├── data-countbyIABref.csv
│   ├── data-countbyinsects.csv
│   ├── data-countbytags.csv
│   ├── data-countmissingabstract.csv
│   ├── data-countpaperbyjournal.csv
│   ├── data-geneticstable.csv
│   ├── data-insectspecies.csv
│   └── data-publicationyear.csv
├── *.c                        # C source files
│   ├── sans_socket.c          # Socket API implementation
│   ├── sans_transport.c       # Transport layer (RUDP)
│   └── socket_map.h           # Socket mapping definitions
├── *.png                      # Graph visualizations
├── results.csv                # Analysis results
└── totalpapers.csv           # Total paper count statistics
```

## 📊 Data Analysis

### Data Files

The project contains comprehensive CSV data files analyzing research papers:

1. **data-countbyauthor.csv** - Paper counts grouped by author
2. **data-countbygenetics.csv** - Research papers categorized by genetic topics
3. **data-countbyIABref.csv** - Papers referenced by IAB (International Association of Biologists)
4. **data-countbyinsects.csv** - Papers grouped by insect species
5. **data-countbytags.csv** - Papers categorized by research tags (Genetics, Life History Trait, Microbiology, etc.)
6. **data-countmissingabstract.csv** - Statistics on papers with missing abstracts
7. **data-countpaperbyjournal.csv** - Paper distribution across scientific journals
8. **data-geneticstable.csv** - Detailed genetics research data
9. **data-insectspecies.csv** - Comprehensive insect species data
10. **data-publicationyear.csv** - Publication trends over time (1992-2024)

### Key Statistics

- **Total Papers Analyzed**: 700+ research papers
- **Publication Years**: 1992-2024
- **Top Journals**: Scientific Reports, Animals, Insects, Frontiers in Microbiology
- **Top Insect Species**: Hermetia illucens (228 papers), Tenebrio molitor (45 papers), Drosophila melanogaster (40 papers)
- **Research Categories**: Genetics, Life History Trait, Microbiology, Ecology, Physiology & Anatomy

## 🔧 Technical Components

### Network Programming (C)

The project includes a custom Socket API for Network Services (SANS) implementation:

- **sans_socket.c**: Core socket functionality supporting:
  - TCP connections
  - RUDP (Reliable UDP) protocol implementation
  - Socket connection management
  - Connection establishment and teardown

- **sans_transport.c**: Transport layer implementation:
  - RUDP data transmission
  - Packet sequencing
  - Reliable data delivery

- **socket_map.h**: Socket mapping structures and definitions

- **http_client.c**: HTTP client implementation using the SANS API

### Features

- **Multi-protocol Support**: TCP and RUDP
- **Reliable UDP**: Custom RUDP implementation with SYN/ACK handshaking
- **Socket Management**: Tracks up to 10 concurrent socket connections
- **HTTP Client**: Full HTTP/1.1 client implementation

## 📈 Visualizations

The project includes several graph visualizations:

- `graph_visualiser-countbyinsects.png` - Insect species distribution
- `graph_visualiser-countpaperbyjournal.png` - Journal publication distribution
- `graph_visualiser-papercount.png` - Overall paper count statistics
- `graph_visualiser-papersbypublications.png` - Publication trends over time
- `graph_visualiser-tags.png` - Research tag distribution
- `graphs.png` - Combined visualization

## 🚀 Getting Started

### Prerequisites

- C compiler (GCC or Clang)
- Make (optional, for building)
- Node.js and npm (if using frontend components)

### Building the C Components

```bash
# Compile socket implementation
gcc -o sans_socket sans_socket.c -I./include

# Compile transport layer
gcc -o sans_transport sans_transport.c -I./include

# Compile HTTP client
gcc -o http_client FADS_Results/http_client.c -I./include
```

### Running the HTTP Client

```bash
./http_client <server_address> <port>
# Then enter HTTP method and path:
# GET /path/to/resource
```

## 📝 Data Analysis Results

The `FADS_Results/` directory contains processed analysis results including:
- Aggregated statistics
- Processed CSV files
- Generated visualizations
- Analysis summaries

## 🔬 Research Focus Areas

1. **Genetics Research**: Studies on insect genetics, gene expression, and genetic diversity
2. **Life History Traits**: Research on insect development, behavior, and life cycles
3. **Microbiology**: Studies on insect microbiomes, symbionts, and bacterial communities
4. **Ecology**: Research on insect ecology, habitats, and environmental interactions
5. **Physiology & Anatomy**: Studies on insect physiology and anatomical structures
6. **Economics**: Research on the economic aspects of insect farming and agriculture

## 📚 Key Research Topics

- Insect farming and sustainable agriculture
- Genetic engineering and insect modification
- Microbiome analysis and symbiosis
- Insect behavior and life history
- Agricultural pest management
- Food and feed production from insects

## 🤝 Contributing

This is a research analysis project. Contributions are welcome for:
- Additional data analysis
- Visualization improvements
- Code enhancements
- Documentation updates

## 📄 License

This project is for research and educational purposes.

## 👤 Author

**Sridhar Reddy**

## 🔗 Repository

[GitHub Repository](https://github.com/fanisridhar/Project--Inseciticides-Farming-)

---

## 📊 Publication Trends

Based on the data analysis:
- **Peak Publication Years**: 2023 (125 papers), 2022 (107 papers), 2021 (111 papers)
- **Research Growth**: Significant increase in publications from 2010 onwards
- **Current Focus**: Active research continues with 34 papers in 2024

## 🐛 Top Research Insects

1. **Hermetia illucens** (Black Soldier Fly) - 228 papers
2. **Tenebrio molitor** (Mealworm) - 45 papers
3. **Drosophila melanogaster** (Fruit Fly) - 40 papers
4. **Bombyx mori** (Silkworm) - 33 papers

## 📖 Top Publishing Journals

1. Scientific Reports - 28 papers
2. Animals - 25 papers
3. Insects - 23 papers
4. Frontiers in Microbiology - 20 papers
5. Journal of Insects as Food and Feed - 20 papers

---

*Last Updated: 2024*

