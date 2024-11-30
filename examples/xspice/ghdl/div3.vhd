library ieee;
use ieee.std_logic_1164.all;

entity divide_by_3 is
    port (
        cout   : out std_logic;  -- Output clock
        clk    : in  std_logic;  -- Input clock
        reset  : in  std_logic   -- Input reset
    );
end entity;

architecture rtl of divide_by_3 is
    signal ring : std_logic_vector (2 downto 0);
begin
    process (clk, reset) begin
        if (reset = '1') then
            ring <= (2 => '1', others => '0');
        elsif (rising_edge(clk)) then
            ring <= ring (1 downto 0) & ring (2);
        end if;
    end process;

    cout <= ring(0) or (ring(1) and clk);
end architecture;

library ieee;
use ieee.std_logic_1164.all;

entity wrapper is
    port (
        clk  : out std_logic;
        cout : out std_logic
    );
end entity;

architecture wrapper_a of wrapper is
    signal reset : std_logic := '1';
    signal iclk  : std_logic := '0';

    component divide_by_3 is
    port (
        cout   :out std_logic;
        clk    :in  std_logic;
        reset  :in  std_logic
    );
    end component;

begin
    dut : divide_by_3
    port map (
        cout   => cout,   -- Output
        clk    => iclk,   -- Input
        reset  => reset   -- Iinput
    );

    clk <= iclk;
    iclk <= not iclk after 1 us;
    reset <= '0' after 2 us;
end architecture;

