e) {
            // Expected
        }
        try {
            StrictMath.scalb((Double) null, 1);
            fail("Should throw NullPointerException");
        } catch (NullPointerException e) {
            // Expected
        }
    }

    /**
     * {@link java.lang.StrictMath#scalb(float, int)}
     * @since 1.6
     */
    @SuppressWarnings("boxing")
    public void test_scalb_FI() {
        // result is normal
        assertEquals(4.1422946304E7f, StrictMath.scalb(1.2345f, 25));
        assertEquals(3.679096698760986E-8f, StrictMath.scalb(1.2345f, -25));
        assertEquals(1.2345f, StrictMath.scalb(1.2345f, 0));
        assertEquals(7868514.304f, StrictMath.scalb(0.2345f, 25));

        float normal = StrictMath.scalb(0.2345f, -25);
        assertEquals(6.98864459991455E-9f, normal);
        // precision kept
        assertEquals(0.2345f, StrictMath.scalb(normal, 25));

        assertEquals(0.2345f, StrictMath.scalb(0.2345f, 0));
        assertEquals(-4.1422946304E7f, StrictMath.scalb(-1.2345f, 25));
        assertEquals(-6.98864459991455E-9f, StrictMath.scalb(-0.2345f, -25));
        assertEquals(2.0f, StrictMath.scalb(Float.MIN_NORMAL / 2, 128));
        assertEquals(64.0f, StrictMath.scalb(Float.MIN_VALUE, 155));
        assertEquals(34, StrictMath.getExponent(StrictMath.scalb(1.0f, 34)));
        assertEquals(3.9999998f, StrictMath.scalb(Float.MAX_VALUE,
                Float.MIN_EXPONENT));

        // result is near infinity
        float halfMax = StrictMath.scalb(1.0f, Float.MAX_EXPONENT);
        assertEquals(1.7014118E38f, halfMax);
        assertEquals(Float.MAX_VALUE, halfMax - StrictMath.ulp(halfMax)
                + halfMax);
        assertEquals(Float.POSITIVE_INFINITY, halfMax + halfMax);
        assertEquals(3.4028233E38f, StrictMath.scalb(1.0f - StrictMath
                .ulp(1.0f), Float.MAX_EXPONENT + 1));
        assertEquals(Float.POSITIVE_INFINITY, StrictMath.scalb(
                1.0f - StrictMath.ulp(1.0f), Float.MAX_EXPONENT + 2));

        halfMax = StrictMath.scalb(-1.0f, Float.MAX_EXPONENT);
        assertEquals(-1.7014118E38f, halfMax);
        assertEquals(-Float.MAX_VALUE, halfMax + StrictMath.ulp(halfMax)
                + halfMax);
        assertEquals(Float.NEGATIVE_INFINITY, halfMax + halfMax);

        assertEquals(Float.POSITIVE_INFINITY, StrictMath.scalb(0.345f, 1234));
        assertEquals(Float.POSITIVE_INFINITY, StrictMath.scalb(44.345E10f, 934));
        assertEquals(Float.NEGATIVE_INFINITY, StrictMath
                .scalb(-44.345E10f, 934));

        assertEquals(Float.POSITIVE_INFINITY, StrictMath.scalb(
                Float.MIN_NORMAL / 2, 400));
        assertEquals(Float.POSITIVE_INFINITY, StrictMath.scalb(Float.MIN_VALUE,
                800));
        assertEquals(Float.POSITIVE_INFINITY, StrictMath.scalb(Float.MAX_VALUE,
                1));
        assertEquals(Float.POSITIVE_INFINITY, StrictMath.scalb(
                Float.POSITIVE_INFINITY, 0));
        assertEquals(Float.POSITIVE_INFINITY, StrictMath.scalb(
                Float.POSITIVE_INFINITY, -1));
        assertEquals(Float.NEGATIVE_INFINITY, StrictMath.scalb(
                Float.NEGATIVE_INFINITY, -1));
        assertEquals(Float.NEGATIVE_INFINITY, StrictMath.scalb(
                Float.NEGATIVE_INFINITY, Float.MIN_EXPONENT));

        // result is subnormal/zero
        int posZeroBits = Float.floatToIntBits(+0.0f);
        int negZeroBits = Float.floatToIntBits(-0.0f);
        assertEquals(posZeroBits, Float.floatToIntBits(StrictMath.scalb(+0.0f,
                Integer.MAX_VALUE)));
        assertEquals(posZeroBits, Float.floatToIntBits(StrictMath.scalb(+0.0f,
                -123)));
        assertEquals(posZeroBits, Float.floatToIntBits(StrictMath.scalb(+0.0f,
                0)));
        assertEquals(negZeroBits, Float.floatToIntBits(StrictMath.scalb(-0.0f,
                123)));
        assertEquals(negZeroBits, Float.floatToIntBits(StrictMath.scalb(-0.0f,
                Integer.MIN_VALUE)));

        assertEquals(Float.MIN_VALUE, StrictMath.scalb(1.0f, -149));
        assertEquals(posZeroBits, Float.floatToIntBits(StrictMath.scalb(1.0f,
                -150)));
        assertEquals(negZeroBits, Float.floatToIntBits(StrictMath.scalb(-1.0f,
                -150)));

        // precision lost
        assertEquals(StrictMath.scalb(21.405f, -154), StrictMath.scalb(21.405f,
                -153));
        assertEquals(Float.MIN_VALUE, StrictMath.scalb(21.405f, -154));
        assertEquals(-Float.MIN_VALUE, StrictMath.scalb(-21.405f, -154));
        assertEquals(posZeroBits, Float.floatToIntBits(StrictMath.scalb(
                21.405f, -155)));
        assertEquals(negZeroBits, Float.floatToIntBits(StrictMath.scalb(
                -21.405f, -155)));
        assertEquals(posZeroBits, Float.floatToIntBits(StrictMath.scalb(
                Float.MIN_VALUE, -1)));
        assertEquals(negZeroBits, Float.floatToIntBits(StrictMath.scalb(
                -Float.MIN_VALUE, -1)));
        assertEquals(Float.MIN_VALUE, StrictMath.scalb(Float.MIN_NORMAL, -23));
        assertEquals(posZeroBits, Float.floatToIntBits(StrictMath.scalb(
                Float.MIN_NORMAL, -24)));
        assertEquals(negZeroBits, Float.floatToIntBits(StrictMath.scalb(
                -Float.MIN_NORMAL, -24)));
        assertEquals(Float.MIN_VALUE, StrictMath.scalb(Float.MAX_VALUE, -277));
        assertEquals(posZeroBits, Float.floatToIntBits(StrictMath.scalb(
                Float.MAX_VALUE, -278)));
        assertEquals(negZeroBits, Float.floatToIntBits(StrictMath.scalb(
                -Float.MAX_VALUE, -278)));
        assertEquals(Float.MIN_VALUE, StrictMath.scalb(Float.MIN_NORMAL / 3,
                -22));
        assertEquals(posZeroBits, Float.floatToIntBits(StrictMath.scalb(
                Float.MIN_NORMAL / 3, -23)));
        assertEquals(negZeroBits, Float.floatToIntBits(StrictMath.scalb(
                -Float.MIN_NORMAL / 3, -23)));
        float subnormal = StrictMath.scalb(Float.MIN_NORMAL / 3, -11);
        assertEquals(1.913E-42f, subnormal);
        // precision lost
        assertFalse(Float.MIN_NORMAL / 3 == StrictMath.scalb(subnormal, 11));

        assertEquals(68747264, Float.floatToIntBits(StrictMath.scalb(
                Float.MIN_VALUE * 153, 23)));
        assertEquals(-2078736384, Float.floatToIntBits(StrictMath.scalb(
                -Float.MIN_VALUE * 153, 23)));

        assertEquals(4896, Float.floatToIntBits(StrictMath.scalb(
                Float.MIN_VALUE * 153, 5)));
        assertEquals(-2147478752, Float.floatToIntBits(StrictMath.scalb(
                -Float.MIN_VALUE * 153, 5)));

        // NaN
        assertTrue(Float.isNaN(StrictMath.scalb(Float.NaN, 1)));
        assertTrue(Float.isNaN(StrictMath.scalb(Float.NaN, 0)));
        assertTrue(Float.isNaN(StrictMath.scalb(Float.NaN, -120)));

        // test for exception
        try {
            StrictMath.scalb((Float) null, (Integer) null);
            fail("Should throw NullPointerException");
        } catch (NullPointerException e) {
            // Expected
        }
        try {
            StrictMath.scalb(1.0f, (Integer) null);
            fail("Should throw NullPointerException");
        } catch (NullPointerException e) {
            // Expected
        }
        try {
            StrictMath.scalb((Float) null, 1);
            fail("Should throw NullPointerException");
        } catch (NullPointerException e) {
            // Expected
        }
    }

    /**
     * java.lang.StrictMath#signum(double)
     */
    public void test_signum_D() {
        assertTrue(Double.isNaN(StrictMath.signum(Double.NaN)));
        assertEquals(Double.doubleToLongBits(0.0), Double
                .doubleToLongBits(StrictMath.signum(0.0)));
        assertEquals(Double.doubleToLongBits(+0.0), Double
                .doubleToLongBits(StrictMath.signum(+0.0)));
        assertEquals(Double.doubleToLongBits(-0.0), Double
                .doubleToLongBits(StrictMath.signum(-0.0)));

        assertEquals(1.0, StrictMath.signum(253681.2187962), 0D);
        assertEquals(-1.0, StrictMath.signum(-125874693.56), 0D);
        assertEquals(1.0, StrictMath.signum(1.2587E-308), 0D);
        assertEquals(-1.0, StrictMath.signum(-1.2587E-308), 0D);

        assertEquals(1.0, StrictMath.signum(Double.MAX_VALUE), 0D);
        assertEquals(1.0, StrictMath.signum(Double.MIN_VALUE), 0D);
        assertEquals(-1.0, StrictMath.signum(-Double.MAX_VALUE), 0D);
        assertEquals(-1.0, StrictMath.signum(-Double.MIN_VALUE), 0D);
        assertEquals(1.0, StrictMath.signum(Double.POSITIVE_INFINITY), 0D);
        assertEquals(-1.0, StrictMath.signum(Double.NEGATIVE_INFINITY), 0D);

    }

    /**
     * java.lang.StrictMath#signum(float)
     */
    public void test_signum_F() {
        assertTrue(Float.isNaN(StrictMath.signum(Float.NaN)));
        assertEquals(Float.floatToIntBits(0.0f), Float
                .floatToIntBits(StrictMath.signum(0.0f)));
        assertEquals(Float.floatToIntBits(+0.0f), Float
                .floatToIntBits(StrictMath.signum(+0.0f)));
        assertEquals(Float.floatToIntBits(-0.0f), Float
                .floatToIntBits(StrictMath.signum(-0.0f)));

        assertEquals(1.0f, StrictMath.signum(253681.2187962f), 0f);
        assertEquals(-1.0f, StrictMath.signum(-125874693.56f), 0f);
        assertEquals(1.0f, StrictMath.signum(1.2587E-11f), 0f);
        assertEquals(-1.0f, StrictMath.signum(-1.2587E-11f), 0f);

        assertEquals(1.0f, StrictMath.signum(Float.MAX_VALUE), 0f);
        assertEquals(1.0f, StrictMath.signum(Float.MIN_VALUE), 0f);
        assertEquals(-1.0f, StrictMath.signum(-Float.MAX_VALUE), 0f);
        assertEquals(-1.0f, StrictMath.signum(-Float.MIN_VALUE), 0f);
        assertEquals(1.0f, StrictMath.signum(Float.POSITIVE_INFINITY), 0f);
        assertEquals(-1.0f, StrictMath.signum(Float.NEGATIVE_INFINITY), 0f);
    }

    /**
     * java.lang.StrictMath#sin(double)
     */
    public void test_sinD() {
        // Test for method double java.lang.StrictMath.sin(double)
        assertTrue("Returned incorrect sine", StrictMath.sin(StrictMath
                .asin(OPP / HYP)) == OPP / HYP);
    }

    /**
     * java.lang.StrictMath#sinh(double)
     */
    public void test_sinh_D() {
        // Test for special situations
        assertTrue(Double.isNaN(StrictMath.sinh(Double.NaN)));
        assertEquals("Should return POSITIVE_INFINITY",
                Double.POSITIVE_INFINITY, StrictMath
                .sinh(Double.POSITIVE_INFINITY), 0D);
        assertEquals("Should return NEGATIVE_INFINITY",
                Double.NEGATIVE_INFINITY, StrictMath
                .sinh(Double.NEGATIVE_INFINITY), 0D);
        assertEquals(Double.doubleToLongBits(0.0), Double
                .doubleToLongBits(StrictMath.sinh(0.0)));
        assertEquals(Double.doubleToLongBits(+0.0), Double
                .doubleToLongBits(StrictMath.sinh(+0.0)));
        assertEquals(Double.doubleToLongBits(-0.0), Double
                .doubleToLongBits(StrictMath.sinh(-0.0)));

        assertEquals("Should return POSITIVE_INFINITY",
                Double.POSITIVE_INFINITY, StrictMath.sinh(1234.56), 0D);
        assertEquals("Should return NEGATIVE_INFINITY",
                Double.NEGATIVE_INFINITY, StrictMath.sinh(-1234.56), 0D);
        assertEquals("Should return 1.0000000000001666E-6",
                1.0000000000001666E-6, StrictMath.sinh(0.000001), 0D);
        assertEquals("Should return -1.0000000000001666E-6",
                -1.0000000000001666E-6, StrictMath.sinh(-0.000001), 0D);
        assertEquals("Should return 5.115386441963859", 5.115386441963859,
                StrictMath.sinh(2.33482), 0D);
        assertEquals("Should return POSITIVE_INFINITY",
                Double.POSITIVE_INFINITY, StrictMath.sinh(Double.MAX_VALUE), 0D);
        assertEquals("Should return 4.9E-324", 4.9E-324, StrictMath
                .sinh(Double.MIN_VALUE), 0D);
    }

    /**
     * java.lang.StrictMath#sqrt(double)
     */
    public void test_sqrtD() {
        // Test for method double java.lang.StrictMath.sqrt(double)
        assertEquals("Incorrect root returned1",
                2, StrictMath.sqrt(StrictMath.pow(StrictMath.sqrt(2), 4)), 0.0);
        assertEquals("Incorrect root returned2", 7, StrictMath.sqrt(49), 0.0);
    }

    /**
     * java.lang.StrictMath#tan(double)
     */
    public void test_tanD() {
        // Test for method double java.lang.StrictMath.tan(double)
        assertTrue(
                "Returned incorrect tangent: ",
                StrictMath.tan(StrictMath.atan(1.0)) <= 1.0
                        || StrictMath.tan(StrictMath.atan(1.0)) >= 9.9999999999999983E-1);
    }

    /**
     * java.lang.StrictMath#tanh(double)
     */
    public void test_tanh_D() {
        // Test for special situations
        assertTrue(Double.isNaN(StrictMath.tanh(Double.NaN)));
        assertEquals("Should return +1.0", +1.0, StrictMath
                .tanh(Double.POSITIVE_INFINITY), 0D);
        assertEquals("Should return -1.0", -1.0, StrictMath
                .tanh(Double.NEGATIVE_INFINITY), 0D);
        assertEquals(Double.doubleToLongBits(0.0), Double
                .doubleToLongBits(StrictMath.tanh(0.0)));
        assertEquals(Double.doubleToLongBits(+0.0), Double
                .doubleToLongBits(StrictMath.tanh(+0.0)));
        assertEquals(Double.doubleToLongBits(-0.0), Double
                .doubleToLongBits(StrictMath.tanh(-0.0)));

        assertEquals("Should return 1.0", 1.0, StrictMath.tanh(1234.56), 0D);
        assertEquals("Should return -1.0", -1.0, StrictMath.tanh(-1234.56), 0D);
        assertEquals("Should return 9.999999999996666E-7",
                9.999999999996666E-7, StrictMath.tanh(0.000001), 0D);
        assertEquals("Should return 0.981422884124941", 0.981422884124941,
                StrictMath.tanh(2.33482), 0D);
        assertEquals("Should return 1.0", 1.0, StrictMath
                .tanh(Double.MAX_VALUE), 0D);
        assertEquals("Should return 4.9E-324", 4.9E-324, StrictMath
                .tanh(Double.MIN_VALUE), 0D);
    }

    /**
     * java.lang.StrictMath#random()
     */
    public void test_random() {
        // There isn't a place for these tests so just stick them here
        assertEquals("Wrong value E",
                4613303445314885481L, Double.doubleToLongBits(StrictMath.E));
        assertEquals("Wrong value PI",
                4614256656552045848L, Double.doubleToLongBits(StrictMath.PI));

        for (int i = 500; i >= 0; i--) {
            double d = StrictMath.random();
            assertTrue("Generated number is out of range: " + d, d >= 0.0
                    && d < 1.0);
        }
    }

    /**
     * java.lang.StrictMath#toRadians(double)
     */
    public void test_toRadiansD() {
        for (double d = 500; d >= 0; d -= 1.0) {
            double converted = StrictMath.toDegrees(StrictMath.toRadians(d));
            assertTrue("Converted number not equal to original. d = " + d,
                    converted >= d * 0.99999999 && converted <= d * 1.00000001);
        }
    }

    /**
     * java.lang.StrictMath#toDegrees(double)
     */
    public void test_toDegreesD() {
        for (double d = 500; d >= 0; d -= 1.0) {
            double converted = StrictMath.toRadians(StrictMath.toDegrees(d));
            assertTrue("Converted number not equal to original. d = " + d,
                    converted >= d * 0.99999999 && converted <= d * 1.00000001);
        }
    }

    /**
     * java.lang.StrictMath#ulp(double)
     */
    @SuppressWarnings("boxing")
    public void test_ulp_D() {
        // Test for special cases
        assertTrue("Should return NaN", Double
                .isNaN(StrictMath.ulp(Double.NaN)));
        assertEquals("Returned incorrect value", Double.POSITIVE_INFINITY,
                StrictMath.ulp(Double.POSITIVE_INFINITY), 0D);
        assertEquals("Returned incorrect value", Double.POSITIVE_INFINITY,
                StrictMath.ulp(Double.NEGATIVE_INFINITY), 0D);
        assertEquals("Returned incorrect value", Double.MIN_VALUE, StrictMath
                .ulp(0.0), 0D);
        assertEquals("Returned incorrect value", Double.MIN_VALUE, StrictMath
                .ulp(+0.0), 0D);
        assertEquals("Returned incorrect value", Double.MIN_VALUE, StrictMath
                .ulp(-0.0), 0D);
        assertEquals("Returned incorrect value", StrictMath.pow(2, 971),
                StrictMath.ulp(Double.MAX_VALUE), 0D);
        assertEquals("Returned incorrect value", StrictMath.pow(2, 971),
                StrictMath.ulp(-Double.MAX_VALUE), 0D);

        assertEquals("Returned incorrect value", Double.MIN_VALUE, StrictMath
                .ulp(Double.MIN_VALUE), 0D);
        assertEquals("Returned incorrect value", Double.MIN_VALUE, StrictMath
                .ulp(-Double.MIN_VALUE), 0D);

        assertEquals("Returned incorrect value", 2.220446049250313E-16,
                StrictMath.ulp(1.0), 0D);
        assertEquals("Returned incorrect value", 2.220446049250313E-16,
                StrictMath.ulp(-1.0), 0D);
        assertEquals("Returned incorrect value", 2.2737367544323206E-13,
                StrictMath.ulp(1153.0), 0D);
    }

    /**
     * java.lang.StrictMath#ulp(float)
     */
    @SuppressWarnings("boxing")
    public void test_ulp_f() {
        // Test for special cases
        assertTrue("Should return NaN", Float.isNaN(StrictMath.ulp(Float.NaN)));
        assertEquals("Returned incorrect value", Float.POSITIVE_INFINITY,
                StrictMath.ulp(Float.POSITIVE_INFINITY), 0f);
        assertEquals("Returned incorrect value", Float.POSITIVE_INFINITY,
                StrictMath.ulp(Float.NEGATIVE_INFINITY), 0f);
        assertEquals("Returned incorrect value", Float.MIN_VALUE, StrictMath
                .ulp(0.0f), 0f);
        assertEquals("Returned incorrect value", Float.MIN_VALUE, StrictMath
                .ulp(+0.0f), 0f);
        assertEquals("Returned incorrect value", Float.MIN_VALUE, StrictMath
                .ulp(-0.0f), 0f);
        assertEquals("Returned incorrect value", 2.028241E31f, StrictMath
                .ulp(Float.MAX_VALUE), 0f);
        assertEquals("Returned incorrect value", 2.028241E31f, StrictMath
                .ulp(-Float.MAX_VALUE), 0f);

        assertEquals("Returned incorrect value", 1.4E-45f, StrictMath
                .ulp(Float.MIN_VALUE), 0f);
        assertEquals("Returned incorrect value", 1.4E-45f, StrictMath
                .ulp(-Float.MIN_VALUE), 0f);

        assertEquals("Returned incorrect value", 1.1920929E-7f, StrictMath
                .ulp(1.0f), 0f);
        assertEquals("Returned incorrect value", 1.1920929E-7f, StrictMath
                .ulp(-1.0f), 0f);
        assertEquals("Returned incorrect value", 1.2207031E-4f, StrictMath
                .ulp(1153.0f), 0f);
        assertEquals("Returned incorrect value", 5.6E-45f, Math
                .ulp(9.403954E-38f), 0f);
    }
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      