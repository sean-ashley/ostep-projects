use clap::Parser;
use anyhow::Result;

/// A simple CLI tool
#[derive(Parser, Debug)]
#[command(name = "your_cli_tool")]
struct Cli {
    /// An example option
    #[arg(short, long)]
    name: String,
}

fn main() -> Result<()> {
    let cli = Cli::parse();
    println!("Hello, {}!", cli.name);
    Ok(())
}